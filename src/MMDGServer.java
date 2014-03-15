import java.net.*;
import java.io.*;
import java.util.Vector;


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
    private final int TCP_PORT = 8080;

    /**
     * Defines how many times per second the MMDG Server should unload the stack
     * of client commands to the application
     */
    private double unloadsPerSecond = 2;

    // CONSTRUCTORS
    /** Creates httpServer, webSocketServer and tcpHandler */
    public MMDGServer() throws IOException {
        allowPrints = true;

        serverIP = getMyIP();
        if (createConfigFile()) {
            print("Created config.js");
        }

        httpServer = new HTTPServer(serverIP, HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);

        // Manage print outs
        httpServer.allowPrints = true;
        webSocketServer.allowPrints = true;

        print("MMDGServer constructor done!\n");
    }

    public void setUnloadsPerSecond(double unloadsPerSecond) {
        if (unloadsPerSecond > 0) {
            this.unloadsPerSecond = unloadsPerSecond;
        }
    }

    // METHODS

    /**
     * Starts the httpServer and webSocketServer. Gets the commandStack from
     * WebSocketServer and tells TCP handler to send the commands 60 times per
     * second.
     * 
     * @throws IOException
     */
    public void run() throws IOException {
        print("Running MMDG-server <http://" + serverIP + ":" + HTTP_PORT
                        + "/mmdg.html>");
        httpServer.listenForNewConnections();
        webSocketServer.connect();

        // will be used to send messages to clients from server
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        Vector<String> commadStack;
        while (true) {

            // print("Write something to the client!");
            // webSocketServer.sendMessage(br.readLine().getBytes());
            // print("Message sent to client");

            // print("Sending message to TCP handler");
            commadStack = webSocketServer.getCommandStack();
            tcpHandler.sendMessages(commadStack);
            webSocketServer.clearCommandStack();
            // print("Sent message to TCP handler");

            // sleep for 1/unloadPerSeconds seconds

            try {
                Thread.sleep((int) (1000 / unloadsPerSecond));
            } catch (InterruptedException e) {
                e.printStackTrace();
                break;
            }
        }
        print("Server stopped");
    }

    public void sendTestMessageViaTCP(String msg) {
        tcpHandler.sendMessage(msg);
    }

    /**
     * Tries to get local host address via Java InetAdress. If that doesn't
     * work, it tries to get the IP address from http://checkip.amazonaws.com/.
     * 
     * @return The IP adress
     */
    private String getMyIP() {
        try {
            // Getting local host address.
            return InetAddress.getLocalHost().getHostAddress();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        
        // This is probably a wierd way to get the IP address...
        String ip = "";
        try {
            URL whatismyip = new URL("http://checkip.amazonaws.com/");
            BufferedReader in = new BufferedReader(new InputStreamReader(
                            whatismyip.openStream()));

            ip = in.readLine(); // you get the IP as a String
        } catch (Exception e) {
            e.printStackTrace();
            return "Couldn't find IP";
        }
        return ip;
    }

    private boolean createConfigFile() {
        try {

            PrintWriter writer = new PrintWriter("public/js/config.js", "UTF-8");

            writer.println("(function (exports) {");
            writer.println("    exports.serverIP = \"" + serverIP + "\";");
            writer.println("    exports.serverWsPort= " + WEB_SOCKET_PORT + ";");
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
