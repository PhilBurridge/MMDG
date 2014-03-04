public class MMDGServer{

    /**
     * This is the HTTP server handler class
     */
    private HTTPServer httpServer;

    /**
     * This is the WebSocket handler class
     */
    private WebSocketServer webSocketServer;

    /**
     * This is the TCP handler class
     */
    private TCPHandler tcpHandler;

    /**
     * The port used by HTTP server
     */
    private final int HTTP_PORT = 1337;

    /**
     * The port used by Web Socket Server
     */
    private final int WEB_SOCKET_PORT = 1338;

    /**
     * The port used by the TCP handler
     */
    private final int TCP_PORT = 8080;

    /**
     * This is a simple constructor
     */
    public MMDGServer() {
        System.out.print("init MMDGServer ... ");
        httpServer = new HTTPServer(HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);
        System.out.println("Done!");

        /* TESTING TCP */
        tcpHandler.sendMessage("Hello TCP");
        
    }

    /**
     * This is just a test method.
     * 
     * @return a string with a greeting message
     */
    public String greet() {
        return "MMDG: Hello world!";
    }
    
    public void hittepa(){
        System.out.println(1);
    }

}
