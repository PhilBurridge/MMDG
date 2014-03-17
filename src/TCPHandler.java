import java.io.*;
import java.net.*;
import java.util.Vector;

/**
 * 
 *
 */
public class TCPHandler extends ConsolePrinter{

    private Socket clientSocket;
    private DataOutputStream outToServer;
    

    public TCPHandler(int tcpPort) {
        try {
            clientSocket = new Socket("localhost", 20501);
            outToServer = new DataOutputStream(clientSocket.getOutputStream());
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendMessage(String message) {
        print("sending TCP message: \"" + message + "\" ");
        try {
            outToServer.writeBytes(message + "\r\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendMessages(Vector<String> commandStack) {
        if (commandStack.size() == 0) {
            print("No message sent.");
            return;
        }

        String messages = "";
        for (int i = 0; i < commandStack.size(); ++i) {
            messages += commandStack.elementAt(i) + ";";
        }
        sendMessage(messages);
    }
}
