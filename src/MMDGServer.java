import java.net.*;
import java.io.*;
import java.util.ArrayList;


public class MMDGServer extends ConsolePrinter{

    /** This is the HTTP server handler class */
    private HTTPServer httpServer;

    /** This is the WebSocket handler class */
    private WebSocketServer webSocketServer;

    /** This is the TCP handler class */
    private TCPHandler tcpHandler;

    /** The local host IP */
    private final String LOCALHOST = "127.0.0.1";

    /** The IP used by the HTTP server */
    private String serverIP = "undefined";

    /** The port used by HTTP server */
    private final int HTTP_PORT = 1337;

    /** The port used by Web Socket Server */
    private final int WEB_SOCKET_PORT = 1338;

    /** The port used by the TCP handler */
    private final int TCP_PORT = 20501;

    /**
     * Defines how many times per second the MMDG Server should unload the stack
     * of client s to the application
     */
    private double unloadsPerSecond = 1000;

    /**
     * A string used for marking the end of a client's "command" sent to the
     * application
     */
    public static final String CMD_DELIMITER = ";";

    /**
     * A string marking the end of each argument in a client's command sent to
     * the application
     */
    public static final String ARG_DELIMITER = " ";

    
    // CONSTRUCTORS
    /** Creates httpServer, webSocketServer and tcpHandler */
    public MMDGServer() throws IOException {

        serverIP = getMyIP();
        if (createConfigFile()) {
            print("Created config.js");
        }

        httpServer = new HTTPServer(serverIP, HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(serverIP, TCP_PORT);

        //print(getLinkToQRCode(600, "000000", "FFFFFF"));
        print("MMDGServer constructor done!\n");
    }

    /**
     * Sets how many times per second the MMDG server will unload the eventstack
     * and send it to the application
     * 
     * @param unloadsPerSecond
     */
    public void setUnloadsPerSecond(double unloadsPerSecond) {
        if (unloadsPerSecond > 0) {
            this.unloadsPerSecond = unloadsPerSecond;
        }
    }

    /**
     * Starts the httpServer and webSocketServer. Gets the commandStack from
     * WebSocketServer and tells TCP handler to send the commands 60 times per
     * second.
     * 
     * @throws IOException
     */
    public void run() throws IOException {

        System.out.println("Running MMDG-server <http://" + serverIP + ":" + HTTP_PORT
                        + "/robbercop.html>");

        httpServer.listenForNewConnections();

        // will start the listener thread for the tcpHandler.
        tcpHandler.listener.start();
        webSocketServer.connect();

        // Read from console in Eclipse
        // BufferedReader br = new BufferedReader(new
        // InputStreamReader(System.in));

        ArrayList<String> commandStack;
        ArrayList<String> appMessageStack;

        String outputString = "";

        while (true) {

            // To send a message from console in eclipse

            // print("Write something to TCP!");
            // tcpHandler.sendMessage(br.readLine());
            appMessageStack = tcpHandler.getMessageStack();
            if (appMessageStack.size() != 0) {
                
                outputString = appMessageStack.get(0);
                forwardMessageFromApp(outputString);

            }
            tcpHandler.clearMessageStack();

            // Send messages from web site to connected application

            commandStack = webSocketServer.getCommandStack();
            tcpHandler.sendToApplication(commandStack);

            // webSocketServer.clearCommandStack();
            // print("Sent message to TCP handler");

            // sleep for 1/unloadPerSeconds seconds
            try {
                // How often stacks get sent to application
                Thread.sleep((int) (1000 / unloadsPerSecond));
            } catch (InterruptedException e) {
                e.printStackTrace();
                break;
            }
        }
        print("Server stopped");
    }
    
    public void sendTestMessageViaTCP(String msg) {
        tcpHandler.sendToApplication(msg);
    }

    /**
     * Calls a with proper parameters to generate a QR code as an image.
     * 
     * @param size
     * Defines the size of the generated image
     * @param color
     * The foreground color as a string defining the hexadecimal color code.
     * @param bgColor
     * The background color as a string defining the hexadecimal color code.
     * @return a link to an image generated on the web, containing the QR code
     * @example getLinkToQRCode(400, "000000", "FFFFFF");
     */
    public String getLinkToQRCode(int size, String color, String bgColor) {

        if (!(isValidColorString(color) && isValidColorString(bgColor))) {
            print("Using default colors for QR code");
            color = "000000";
            bgColor = "FFFFFF";
        }

        String link = "";
        link += "http://api.qrserver.com/v1/create-qr-code/";
        link += "?color=" + color + "&bgcolor=" + bgColor;
        link += "&data=http%3A%2F%2F" + serverIP + "%3A" + HTTP_PORT
                        + "%2Fmmdg.html";
        link += "&qzone=1&margin=0&";
        link += "size=" + size + "x" + size + "&ecc=L";
        return link;
    }
    
    private boolean forwardMessageFromApp(String outputString){
        if(outputString.startsWith("id=")){
            int delimiter_pos = outputString.indexOf(ARG_DELIMITER);
            String receiverID = outputString.substring(3, delimiter_pos);
            String msg = outputString.substring(delimiter_pos+1);
            
            if (receiverID.equalsIgnoreCase("all")) {
                print("Sending \"" + msg + "\" to all clients");
                webSocketServer.sendMessageToAllClients(msg);
            }
            else if(receiverID.equalsIgnoreCase("server")){
                if (msg.equalsIgnoreCase("appConnected")){
                    appConnected();
                }
            }
            else {
                try{
                    int id = Integer.parseInt(receiverID);
                    print("Sending " + msg + " to clients with id " + id);
                    webSocketServer.sendMessageToClient(id, msg);
                }catch(Exception e){
                    e.printStackTrace();
                    print("Error: Receiver index is not an integer!");
                    return false;
                }
            }
            return true;
        }
        else{
            print("Error: Message from appilcation has no reciever!");
            print("Message from appilcation should start with: id=<int> or \"all\"");
            return false;
        }
        
    }
    
    private void appConnected(){
        webSocketServer.reconnectClientsToApplication();
    }
    
    /**
     * Checks whether or not a string is a valid color string in hexadecimal
     * format. Example of a correct string: "0359AF".
     * 
     * @param s
     * The string to be checked
     * @return true is the string is a valid color string
     */
    private boolean isValidColorString(String s) {
        if (s.length() != 6) {
            print("Error! Color string length = " + s.length());
            return false;
        }
        char c;
        for (int i = 0; i < 6; ++i) {
            c = s.charAt(i);
            if (!(('0' <= c && c <= '9') || ('A' <= c && c <= 'F'))) {
                print("Error: Color string cannot contain " + c);
            }
        }
        return true;
    }

    /**
     * Tries to get local host address via Java InetAdress. If that doesn't
     * work, it tries to get the IP address from http://checkip.amazonaws.com/.
     * 
     * @return The local host IP address, or public IP address.
     */
    private String getMyIP() {
        try {
            // Getting local host address.
            return InetAddress.getLocalHost().getHostAddress();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }

        // This is probably a wierd way to get the IP address...
        try {
            URL whatismyip = new URL("http://checkip.amazonaws.com/");
            BufferedReader in = new BufferedReader(new InputStreamReader(
                            whatismyip.openStream()));

            return in.readLine(); // you return the IP as a String
        } catch (Exception e) {
            e.printStackTrace();
            return "Couldn't find IP";
        }
    }

    /**
     * Creates the file "public/js/config.js" which will be sent to all clients
     * and used in order to connect to the web socket server. The file contains
     * the info about the web socket IP and port.
     * 
     * @return True if the file was successfully created, else false.
     */
    private boolean createConfigFile() {
        try {

            PrintWriter writer = new PrintWriter("public/js/config.js", "UTF-8");

            writer.println("(function (exports) {");
            writer.println("    exports.serverIP = \"" + serverIP + "\";");
            writer.println("    exports.serverWsPort= " + WEB_SOCKET_PORT + ";");
            writer.println("    exports.arg_delimiter= \"" + ARG_DELIMITER
                            + "\";");
            writer.println("})(typeof exports === 'undefined' ? this['config']={} : exports);");

            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}
