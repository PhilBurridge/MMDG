import java.io.*;
import java.net.*;
import java.util.Vector;


/**
 * This class handles the TCP handling between the MMDG server and the
 * application
 * 
 */
public class TCPHandler extends ConsolePrinter{

    /** The Socket to use for TCP communication with application */
    private Socket clientSocket;

    /** The object which writes the messages to the application */
    private DataOutputStream outToServer;

    /** The objects which reads messages from the application */
    private BufferedReader inFromApplication;

    /**
     * Creates the socket and the outputStream. Exceptions are handled if
     * something goes badly
     * 
     * @param tcpPort
     * the port number
     */
    public TCPHandler(int tcpPort) {
        try {
            clientSocket = new Socket("localhost", tcpPort);
            outToServer = new DataOutputStream(clientSocket.getOutputStream());
            inFromApplication = new BufferedReader(new InputStreamReader(
                            clientSocket.getInputStream()));
        } catch (Exception e) {
            print("ERROR - could not connect to Application!");
            e.printStackTrace();
        }
    }

    /**
     * Sends a message to the connected TCP application
     * 
     * @param message
     * The message to be sent
     */
    public void sendMessage(String message) {
        print("sending TCP message: \"" + message + "\"");
        try {
            outToServer.writeBytes(message + "\r\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Sends a stack of messages to the connected TCP application.
     * 
     * @param commandStack
     * The messages to be sent. (Vector of Strings)
     */
    public void sendMessages(Vector<String> commandStack) {
        // If command stack is empty, don't send anything
        if (commandStack.size() == 0) {
            // print("No message sent.");
            return;
        }

        // If commandstack is not empty, collect all commands in one string
        // Currently we are using ";" as delimiter.
        String messages = "";
        for (int i = 0; i < commandStack.size(); ++i) {
            messages += commandStack.elementAt(i) + "";
        }
        sendMessage(messages);
    }

    /**
     * Receive messages from the connected application using a thread
     */

    public void receiveMessages() {
        Thread appThread = new Thread(new Runnable() {

            @Override
            public void run() {
                String appMessages;
                try {
                    while (true) {
                        appMessages = inFromApplication.readLine();
                        if(appMessages != null)
                            print("Message from application: " + appMessages);
                    }
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        
        appThread.start();
        print("Started thread used to listen to application...");
    }
}
