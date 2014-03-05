import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;


/**
 * Handles the websocket connections.
 */
public class WebSocketServer{

    private ServerSocket serverSocket;
    private Socket socket;

    private Vector<String> commandStack;
    VirtualClient cs;

    public WebSocketServer(int websocketPort) throws IOException {
        commandStack = new Vector<String>();
        serverSocket = new ServerSocket(websocketPort);
        connect();
    }

    public void listenToWebSocketMessages() {
        cs = new VirtualClient(this, 1);
        cs = new VirtualClient(this, 2);
    }

    public void addCommand(String command) {
        commandStack.add(command);
    }

    public Vector<String> getCommandStack() {
        return commandStack;
    }

    public void clearCommandStack() {
        commandStack.clear();
    }

    private void connect() throws IOException {
        System.out.println("Listening");
        socket = serverSocket.accept();
        System.out.println("Got connection");
    }
}
