import java.util.Vector;


public class MMDGServer{

    /** This is the HTTP server handler class */
    private HTTPServer httpServer;

    /** This is the WebSocket handler class */
    private WebSocketServer webSocketServer;

    /** This is the TCP handler class */
    private TCPHandler tcpHandler;

    /** The IP used by HTTP server */
    private final String HOST = "127.0.0.1";
    
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
    public MMDGServer() {
        System.out.print("init MMDGServer ... ");
        httpServer = new HTTPServer(HOST, HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);
        System.out.println("Done!");
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
     */
    public void run() {
        int maxTime = 10; //
        System.out.println("Server run for " + maxTime + " seconds.");

        httpServer.listenForNewConnections();
        webSocketServer.listenToWebSocketMessages();
        Vector<String> commadStack;
        int unloads = 0;
        while (true) {

            commadStack = webSocketServer.getCommandStack();
            tcpHandler.sendMessages(commadStack);
            webSocketServer.clearCommandStack();

            // sleep for 1/unloadPerSeconds seconds
            try {
                Thread.sleep((int) (1000 / unloadsPerSecond));
            } catch (InterruptedException e) {
                e.printStackTrace();
                break;
            }
            unloads++;
            if (unloads / unloadsPerSecond >= 10) break;
        }
        System.out.println("Server stopped");
    }
   /* public void sendTestMessageViaTCP(String msg) {
        tcpHandler.sendMessage(msg);
    }*/

}
