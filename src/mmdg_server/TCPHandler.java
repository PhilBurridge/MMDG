package mmdg_server;
import java.io.*;
import java.net.*;
import java.util.ArrayList;


/**
 * This class handles the TCP handling between the MMDG server and the
 * application
 * 
 */
public class TCPHandler extends ConsolePrinter implements Runnable{

    public Thread listener = new Thread(this);

    /** The Socket to use for TCP communication with application */
    private Socket appSocket;

    /** The object which writes the messages to the application */
    private DataOutputStream outToApplication;

    /** The objects which reads messages from the application */
    private BufferedReader inFromApplication;

    /** Address object that saves host and port of server */
    private InetSocketAddress socketAddress;

    /** ArrayList of messages that the application wants to send to the clients. */
    private ArrayList<String> messagesFromApp;

    /**
     * When not connected to an application, this specifies the number of
     * milliseconds to wait before trying to connect next time
     */
    private int connectIntervall = 1000;

    /**
     * Shows whether server is connected to application. Null if it hasn't tried
     * to connect yet
     */
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
        messagesFromApp = new ArrayList<String>();
        appSocket = new Socket();

    }

    /**
     * The TCPHandler class stores incoming messages from the Application in an
     * ArrayList. The server gets the messages by calling this method.
     * 
     * @return An ArrayList of messages from the application
     */
    public ArrayList<String> getMessageStack() {
        return messagesFromApp;
    }
    
    /**
     * The TCPHandler class stores incoming messages from the Application in an
     * ArrayList. The ArrayList is cleared when this function is called
     */
    public void clearMessageStack(){
        messagesFromApp.clear();
    }

    /**
     * Sends a ASCII string to the application.
     * 
     * @param message
     * The message to be sent
     */
    public void sendToApplication(String message) {
        print("Sending message to app: \"" + message + "\"");

        /*
         * If the connection to the app is not established, TCPhandler will
         * leave a message in ClientMessages that commands will not be received.
         */
        if (!appSocket.isConnected() || appSocket.isClosed()) {
            messagesFromApp.add("Your message cannot be recieved by the application right now");
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

                // Gets the status and prints a message if there is change
                boolean contact = appSocket.isConnected();
                notifyAppConnectionChange(contact);

                /*
                 * Checks if we have established a connection to the application
                 * AND THEN if we can read a message. Must be in that order
                 * because if we dont have a connection, we have not initialized
                 * the streams yet.
                 */
                if (contact && (message = inFromApplication.readLine()) != null) {
                    print("Received message \"" + message + "\"");
                    
                    // If we want to be able to send messages from app to
                    // clients
                    if (message.startsWith("id=")) {
                        messagesFromApp.add(message);
                    }else if (message.startsWith("ping")) {
                        messagesFromApp.add(message);
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
        appSocket = new Socket();
        try {
            /*
             * trying to connect to app. A timeout in milliseconds is specified
             * in the second argument. This function will block the thread
             * untill a connection is reached. This is the phone.
             */
            appSocket.connect(socketAddress, connectIntervall);

            // This is the mouth
            outToApplication = new DataOutputStream(
                            appSocket.getOutputStream());

            // This is the ear
            inFromApplication = new BufferedReader(new InputStreamReader(
                            appSocket.getInputStream()));
        } catch (IOException e) {
            // print("Could not connect to Application.");
            // e.printStackTrace();
        }
    }

    /**
     * Will close the socket safely
     */
    private void closeSocket() {
        if (appSocket != null) {
            try {
                appSocket.close();
            } catch (IOException e) {
                print("closing socket exception");
            }
        }
    }

    /**
     * Prints a message if there is a change of the connection status.
     * 
     * @param newStatus
     * The new status of the connection
     */
    private void notifyAppConnectionChange(boolean newStatus) {
        // If status has been set, and the new status is the same, do nothing
        if (appConnected != null && appConnected == newStatus) return;

        if (newStatus == true) {
            print("Connection established!");
            messagesFromApp.add("id=server" + MMDGServer.ARG_DELIMITER + "appConnected");
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

