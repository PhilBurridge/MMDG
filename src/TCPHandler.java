import java.io.*;
import java.net.*;
import java.util.Vector;


/**
 * This class handles the TCP handling between the MMDG server and the
 * application
 * 
 */
public class TCPHandler extends ConsolePrinter implements Runnable{

    public Thread listener = new Thread(this);

    /** The Socket to use for TCP communication with application */
    private Socket clientSocket;

    /** The object which writes the messages to the application */
    private DataOutputStream outToApplication;

    /** The objects which reads messages from the application */
    private BufferedReader inFromApplication;

    /** Address object that saves host and port of server */
    private InetSocketAddress socketAddress;

    /** Vector of messages that the application wants to send to the clients. */
    private Vector<String> clientMessages;

    /**
     * When not connected to an application, this specifies the number of
     * milliseconds to wait before trying to connect next time
     */
    private int connectIntervall = 1000;

    private Boolean appConnected = null;

    /**
     * Creates the socket and the outputStream. Exceptions are handled if
     * something goes badly
     * 
     * @param tcpPort
     * the port number
     */
    public TCPHandler(String host, int tcpPort) {

        socketAddress = new InetSocketAddress(host, tcpPort);
        clientMessages = new Vector<String>();
        clientSocket = new Socket();

    }

    /**
     * The TCPHandler class stores incoming messages from the Application in a
     * Vector. The server gets the messages by calling this method.
     * 
     * @return A vector of messages from the application
     */
    public Vector<String> getClientMessages() {
        return clientMessages;
    }

    /**
     * Sends a ASCII string to the application.
     * 
     * @param message
     * The message to be sent
     */
    public void sendMessage(String message) {
        print("sending TCP message: \"" + message + "\"");

        /*
         * If the connection to the app is not established, TCPhandler will
         * leave a message in ClientMessages that commands will not be received.
         */
        if (!clientSocket.isConnected() || clientSocket.isClosed()) {
            clientMessages.add("Your message cannot be recieved by the application right now");
            print("Wants to send Clients a message that Application connection is closed");
        } else {
            try {
                outToApplication.writeBytes(message + "\r\n");
            } catch (IOException e) {
                print("outStream is not established");
                e.printStackTrace();
            }
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
    @Override
    public void run() {
        print("Trying to connect to application");

        // String where messages from application is temporary stored
        String message = "";

        // Call tcpHandlerObject.listener.inerrupt() to turn of this thread
        // safely
        while (!Thread.interrupted()) {
            try {
                
                //Gets the status and prints a message if there is change
                boolean contact = clientSocket.isConnected();
                notifyAppConnectionChange(contact);
                
                /*
                 * Checks if we have established a connection to the application
                 * AND THEN if we can read a message. Must be in that order
                 * because if we dont have a connection, we have not initialized
                 * the streams yet.
                 */
                if (contact && (message = inFromApplication.readLine()) != null) {

                    // If we want to be able to send messages from app to
                    // clients
                    if (message.startsWith("id=")) {
                        clientMessages.add(message);
                    } else {
                        print(message);
                    }

                } else {
                    /*
                     * If we have not established a connection to app, or if we
                     * are receiving null messages. Could be a problem if we
                     * were to get null messages for any other reason than
                     * losing the connection
                     */
                    // print("client receives null messages or is not connected to application");
                    tryConnectSocket();
                }
            } catch (IOException e) {
                print("read application message exception");
                e.printStackTrace();
            }
        }
    }

    /**
     * Will try to reconnect to the application. This function will block the
     * thread until established connection.
     */
    private void tryConnectSocket() {
        /*
         * If we lose connection with a socket, you have to close it and make a
         * new. There seems to be no way to reconnect strangely enough.
         */
        closeSocket();

        // print("Trying to reconnect");
        clientSocket = new Socket();
        try {
            /*
             * trying to connect to app. A timeout in milliseconds is specified
             * in the second argument. This function will block the thread
             * untill a connection is reached. This is the phone.
             */
            clientSocket.connect(socketAddress, connectIntervall);

            // This is the mouth
            outToApplication = new DataOutputStream(
                            clientSocket.getOutputStream());

            // This is the ear
            inFromApplication = new BufferedReader(new InputStreamReader(
                            clientSocket.getInputStream()));
        } catch (IOException e) {
            // print("Could not connect to Application.");
            // e.printStackTrace();
        }
    }

    /**
     * Will close the socket safely
     */
    private void closeSocket() {
        if (clientSocket != null) {
            try {
                clientSocket.close();
            } catch (IOException e) {
                print("closing socket exception");
            }
        }
    }
    
    /**
     * Prints a message if there is a change of the connection status.
     * @param newStatus The new status of the connection
     */
    private void notifyAppConnectionChange(boolean newStatus) {
        // If status has been set, and the new status is the same, do nothing
        if (appConnected != null && appConnected == newStatus) return;

        if (newStatus == true) {
            print("Connection established!");
        } else {
            print("No connection to application");
            print("New attempts to connect will be made every "
                            + connectIntervall
                            + " milliseconds in the background...");
        }

        // Update status
        appConnected = newStatus;

    }

}
