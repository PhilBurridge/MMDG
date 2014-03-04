public class MMDGServer{

    /**
     *  This is the HTTP server handler class
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
    * This is a simple constructor
    */
    public MMDGServer(){
        System.out.print("init MMDGServer ... ");
        httpServer = new HTTPServer();
        webSocketServer = new WebSocketServer();
        tcpHandler = new TCPHandler();
        System.out.println("Done!");
    }
    
    /**
     * This is just a test method.
     * @return a string with a greeting message
     */
    public String greet(){
        return "MMDG: Hello world!";
    }

}
