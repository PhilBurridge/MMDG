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

    /** The IP used by HTTP server */
    private final String LOCALHOST = "127.0.0.1";
    private final String HOST = get_my_IP();

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
        print("init MMDGServer ... ");
        print("Server IP is: " + HOST);
        httpServer = new HTTPServer(HOST, HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);
        print("MMDGServer constructed!");

        // Manage print outs
        httpServer.allowPrints = true;
        webSocketServer.allowPrints = true;

        System.out.println(getLinkToQRCode(400, 400, "000000", "FFFFFF"));
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

        print("Listening to HTTP requests...");
        httpServer.listenForNewConnections();
        webSocketServer.connect();

        // will be used to send messages to clients from server
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        Vector<String> commadStack;
        while (true) {

            // print("Write something to the client!");
            // webSocketServer.sendMessage(br.readLine().getBytes());
            // print("Message sent to client");

            print("Sending message to TCP handler");
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
     * Calls a with proper parameters to generate a QR code as an image.
     * 
     * @param width
     * Defines the width of the generated image
     * @param height
     * Defines the height of the generated image
     * @param color
     * The foreground color as a string defining the hexadecimal color code.
     * @param bgColor
     * The background color as a string defining the hexadecimal color code.
     * @return a link to an image generated on the web, containing the QR code
     * @example getLinkToQRCode(400, 400, "000000", "FFFFFF");
     */
    public String getLinkToQRCode(int width, int height, String color,
                    String bgColor) {
        if (!(isValidColorString(color) && isValidColorString(bgColor))) {
            System.out.println("Using default colors for QR code");
            color = "000000";
            bgColor = "FFFFFF";
        }

        String link = "";
        link += "http://api.qrserver.com/v1/create-qr-code/";
        link += "?color=" + color + "&bgcolor=" + bgColor;
        link += "&data=http%3A%2F%2F" + HOST + "%3A" + HTTP_PORT
                        + "%2Fmmdg.html";
        link += "&qzone=1&margin=0&size=400x400&ecc=L";
        return link;
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
            System.out.println("Error! Color string length = " + s.length());
            return false;
        }
        char c;
        for (int i = 0; i < 6; ++i) {
            c = s.charAt(i);
            if (!(('0' <= c && c <= '9') || ('A' <= c && c <= 'F'))) {
                System.out.println("Error: Color string cannot contain " + c);
            }
        }
        return true;
    }

    static private String get_my_IP() {
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
}
