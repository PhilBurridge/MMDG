import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;


public class WebSocketServer{
    
    private ServerSocket serverSocket;
    private Socket socket;

    public WebSocketServer(int websocketPort) throws IOException{
        serverSocket = new ServerSocket(websocketPort);
        connect();
    }
    
    private void connect() throws IOException {
        System.out.println("Listening");
        socket = serverSocket.accept();
        System.out.println("Got connection");

        }
}
