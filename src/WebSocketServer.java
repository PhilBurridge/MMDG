import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Vector;
import org.apache.commons.codec.binary.Base64;


// good tutorial:
// http://stackoverflow.com/questions/12702305/using-html5-client-with-a-server-in-java

/**
 * Handles the websocket connections.
 */
public class WebSocketServer extends ConsolePrinter{

    public static final int MASK_SIZE = 4;
    public static final int SINGLE_FRAME_UNMASKED = 0x81;

    private static int next_client_id = 0;

    /** server socket that waits and possibly responds to requests */
    private ServerSocket serverSocket;

    /** Handles all the client sockets */
    private HashMap<Integer, ClientHandler> clientHandlers;

    /**
     * a buffer of messages that will fill upp until MMDGServer forwards it to
     * the TCPHandler
     */
    private Vector<String> commandStack;

    /** initiate commandStack and server socket */
    public WebSocketServer(int websocketPort) throws IOException {
        serverSocket = new ServerSocket(websocketPort);
        clientHandlers = new HashMap<Integer, ClientHandler>();
        commandStack = new Vector<String>();
    }

    public synchronized void addCommand(String command) {
        commandStack.add(command + MMDGServer.CMD_DELIMITER);
        print("Added \"" + command + "\" to command stack");
    }

    public synchronized Vector<String> getCommandStack() {
        Vector<String> commandStackCopy = new Vector<String>(commandStack);
        commandStack.clear();
        return commandStackCopy;
    }

    public void clearCommandStack() {
        commandStack.clear();
    }

    /**
     * will listen for client connections in a separate thread, create a new
     * socket and add it to a socket array/vector. so far it is not in a
     * seperate thread, and it only has 1 socket, only 1 person can connect at a
     * time.
     */
    public void connect() throws IOException {
        Thread connectThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (true) {
                        //print("Waiting for connections ...");
                        Socket socket = serverSocket.accept();
                        System.out.println();
                        //print("Connecting!");

                        removeDeadClientHandlers();
                        if (handshake(socket)) {
                            addHandlerForClient(socket);
                        }
                    }
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        connectThread.start();

    }

    public void sendMessageToClient(int id, String msg) {
        try {
            clientHandlers.get(id).sendMessage(msg.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendMessageToAllClients(String msg) {
        try {
            Iterator<Entry<Integer, ClientHandler>> it = clientHandlers
                            .entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry<Integer, ClientHandler> pairs = (Map.Entry<Integer, ClientHandler>) it
                                .next();
                pairs.getValue().sendMessage(msg.getBytes());
                //it.remove(); // avoids a ConcurrentModificationException
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /** Server socket and client sockets does a firm and manly handshake. */
    private boolean handshake(Socket socket) throws IOException {
        PrintWriter out = new PrintWriter(socket.getOutputStream());
        BufferedReader in = new BufferedReader(new InputStreamReader(
                        socket.getInputStream()));

        HashMap<String, String> keys = new HashMap<String, String>();
        String str;

        // Reading client handshake
        //print("Reading client handshake");
        while (!(str = in.readLine()).equals("")) {
            String[] s = str.split(": ");
            // print(str);
            if (s.length == 2) {
                keys.put(s[0], s[1]);
            }
        }
        // print("hashmap"+keys.toString());

        // Do what you want with the keys here, we will just use
        // "Sec-WebSocket-Key"
        String hash;
        try {
            new Base64();
            hash = Base64.encodeBase64String(MessageDigest
                            .getInstance("SHA-1")
                            .digest((keys.get("Sec-WebSocket-Key") + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")
                                            .getBytes()));
        } catch (NoSuchAlgorithmException ex) {
            ex.printStackTrace();
            return false;
        }

        // Write handshake response
        String response = "HTTP/1.1 101 Switching Protocols\r\n"
                        + "Upgrade: websocket\r\n" + "Connection: Upgrade\r\n"
                        + "Sec-WebSocket-Accept: " + hash + "\r\n" + "\r\n";
        //print("Writing response");
        // print(response);
        out.write(response);
        out.flush();

        return true;
    }

    private void addHandlerForClient(Socket socket) {
        ClientHandler ch = new ClientHandler(socket, get_next_client_id());

        clientHandlers.put(ch.id, ch);
        ch.listenToClient();

        print("Number of clients: " + clientHandlers.size());
        print("Added client with ID " + ch.id);
    }

    private void removeDeadClientHandlers() {

        Iterator<Entry<Integer, ClientHandler>> it = clientHandlers.entrySet()
                        .iterator();
        while (it.hasNext()) {
            Map.Entry<Integer, ClientHandler> pairs = (Map.Entry<Integer, ClientHandler>) it
                            .next();

            if (!pairs.getValue().alive) {
                clientHandlers.remove(pairs.getKey());
            }

            // it.remove(); // avoids a ConcurrentModificationException
        }
    }

    private static int get_next_client_id() {
        ++next_client_id;
        return next_client_id;
    }

    private int getSizeOfPayload(byte b) {
        // Must subtract 0x80 from masked frames
        return ((b & 0xFF) - 0x80);
    }

    private byte[] unMask(byte[] mask, byte[] data) {
        for (int i = 0; i < data.length; i++) {
            data[i] = (byte) (data[i] ^ mask[i % mask.length]);
        }
        return data;
    }

    // private void convertAndPrint(byte[] bytes) {
    // StringBuilder sb = new StringBuilder();
    // for (byte b : bytes) {
    // sb.append(String.format("%02X ", b));
    // }
    // print(sb.toString());
    // }

    /**
     * This class is for handling clients. One instance of this class takes care
     * of one client. It uses one separate thread for each client, which might
     * not be the best solution. In the future we might use a limited number of
     * threads in a so called "thread pool" and maybe gain performance?
     * 
     * @pros: We can have ID:s for clienthandlers instead of clients.
     * @cons: Performance might decrease drastically when many clients connect
     * 
     */
    private class ClientHandler extends ConsolePrinter{
        /**
         * An instance of the client socket. With this we can read and write to
         * the client
         */
        private Socket clientSocket;

        /** The thread used to listen to this handlers particular client */
        private Thread listenerTread;

        /** A clientHandler is alive as long as it has a connected client */
        private boolean alive;

        /** The ID of the client */
        private int id;

        /**
         * Constructrs a handler for the current constructor, with a specified
         * ID.
         * 
         * @param clientSocket
         * clientSocket to be handled
         * @param id
         */
        public ClientHandler(Socket clientSocket, int id) {
            this.clientSocket = clientSocket;
            this.id = id;
            alive = true;
            allowPrints = true;
        }

        /**
         * Reads a chunk of bytes from the clientSockets inputstream.
         * 
         * @param numOfBytes
         * Number of bytes to read
         * @return Returns the read bytes
         * @throws IOException
         */
        private byte[] readBytes(int numOfBytes) throws IOException {
            // print("numOfBytes = " + numOfBytes);
            byte[] b = new byte[numOfBytes];
            clientSocket.getInputStream().read(b);
            if (b.length < 0) {
                print("Negative byte size..");
                stop();
                return null;
            }
            return b;
        }

        /**
         * Use this method to stop this client Handler from working.
         * 
         * @throws IOException
         */
        private void stop() throws IOException {
            print("Client " + id + " closed!");
            clientSocket.close();
            alive = false;
            // listenerTread.join();
        }

        /**
         * Receives a message from the client socket
         * 
         * @return The received message from client
         * @throws IOException
         */
        public String reiceveMessage() throws IOException {
            byte[] buf = readBytes(2);
            // print("Headers:");
            // convertAndPrint(buf);
            int opcode = buf[0] & 0x0F;
            if (opcode == 8) {
                print("Client want to close connection");
                stop();
                return null;
            } else {
                final int payloadSize = getSizeOfPayload(buf[1]);
                if (payloadSize == -128) {
                    print("playloadSize is -128");
                    stop();
                    return "disconnected";
                }
                buf = readBytes(MASK_SIZE + payloadSize);
                // print("Payload:");
                // convertAndPrint(buf);
                buf = unMask(Arrays.copyOfRange(buf, 0, 4),
                                Arrays.copyOfRange(buf, 4, buf.length));
                String message = new String(buf);
                return message;
            }
        }

        /**
         * Creates a thread for listening for client messages.
         */
        public void listenToClient() {
            listenerTread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        while (alive) {
                            String msg = reiceveMessage();
                            print("Recieved from client " + id + ": " + msg + "\". Adding as command.");
                            addCommand("id=" + id + MMDGServer.ARG_DELIMITER + msg);
                        }
                        print("The listening thread of clientHandler " + id
                                        + " is done");
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            });
            listenerTread.start();
            //print("Started thread used to listen to client messages...");
        }

        public void sendMessage(byte[] msg) throws IOException {
            print("Sending to client");
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            BufferedOutputStream os = new BufferedOutputStream(
                            clientSocket.getOutputStream());
            baos.write(SINGLE_FRAME_UNMASKED);
            baos.write(msg.length);
            baos.write(msg);
            baos.flush();
            baos.close();
            // convertAndPrint(baos.toByteArray());
            os.write(baos.toByteArray(), 0, baos.size());
            os.flush();
        }
    }

}
