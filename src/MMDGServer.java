import java.util.Vector;


public class MMDGServer{

    /** This is the HTTP server handler class */
    private HTTPServer httpServer;

    /** This is the WebSocket handler class */
    private WebSocketServer webSocketServer;

    /** This is the TCP handler class */
    private TCPHandler tcpHandler;

    /** The port used by HTTP server */
    private final int HTTP_PORT = 1337;

    /** The port used by Web Socket Server */
    private final int WEB_SOCKET_PORT = 1338;

    /** The port used by the TCP handler */
    private final int TCP_PORT = 8080;
    
    /** */
    private int unloadsPerSecond = 2;  

    
    // CONSTRUCTORS
    /** Creates httpServer, webSocketServer and tcpHandler */
    public MMDGServer() {
        System.out.print("init MMDGServer ... ");
        httpServer = new HTTPServer(HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);
        System.out.println("Done!");
    }

    // METHODS

    /**
     * Starts the httpServer and webSocketServer. Gets the commandStack from
     * WebSocketServer and tells TCP handler to send the commands 60 times per
     * second. 
     */
    public void run() {
        httpServer.listenForNewConnections();
        webSocketServer.listenToWebSocketMessages();
        
        int unloads = 0;
        while (true) {
            
            Vector<String> commadStack = webSocketServer.getCommandStack();
            tcpHandler.sendMessages(commadStack);
            webSocketServer.clearCommandStack();
            
            // sleep for 1/unloadPerSeconds seconds
            try {
                Thread.sleep(1000/unloadsPerSecond);
            } catch (InterruptedException e) {
                e.printStackTrace();
                break;
            }
            unloads++;
            if (unloads/unloadsPerSecond >= 5)
                break;
        }
        System.out.println("Server stopped");
    }

    public void sendTestMessageViaTCP(String msg) {
        tcpHandler.sendMessage(msg);
    }

}
