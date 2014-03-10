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
    public MMDGServer() throws IOException{
        allowPrints = true;
        print("init MMDGServer ... ");
        print("Server IP is: " + HOST);
        httpServer = new HTTPServer(HOST, HTTP_PORT);
        webSocketServer = new WebSocketServer(WEB_SOCKET_PORT);
        tcpHandler = new TCPHandler(TCP_PORT);
        print("MMDGServer constructed!");
        
        webSocketServer.allowPrints = false;
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
     * @throws IOException 
     */
    public void run() throws IOException {
        
        print("Listening to HTTP requests...");
        httpServer.listenForNewConnections();
        
        webSocketServer.connect();
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
            print("Write something to the client!");
            webSocketServer.sendMessage(br.readLine().getBytes());
        }
        
        
        
        /*
        int maxTime = 10; //
        print("Server run for " + maxTime + " seconds.");
        
        httpServer.listenForNewConnections();
        webSocketServer.listenToWebSocketMessages();
        Vector<String> commadStack;
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
        }
        print("Server stopped");*/
    }
    
    public void sendTestMessageViaTCP(String msg) {
        tcpHandler.sendMessage(msg);
    }
    
    static private String get_my_IP(){
        String ip = "";
        try{
            URL whatismyip = new URL("http://checkip.amazonaws.com/");
            BufferedReader in = new BufferedReader(new InputStreamReader(
                            whatismyip.openStream()));
    
            ip = in.readLine(); //you get the IP as a String
        }catch(Exception e){
            e.printStackTrace();
            return "Couldn't find IP";
        }
        return ip;
    }
}
