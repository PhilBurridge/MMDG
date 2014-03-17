import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.HashMap;
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

    /** server socket that waits and possibly responds to requests */
    private ServerSocket serverSocket;
    /** a client socket, endpoint for communication */
    private Socket socket;

    /**
     * a buffer of messages that will fill upp until MMDGServer forwards it to
     * the TCPHandler
     */
    private Vector<String> commandStack;

    /** initiate commandStack and server socket */
    public WebSocketServer(int websocketPort) throws IOException {
        commandStack = new Vector<String>();
        serverSocket = new ServerSocket(websocketPort);
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

    /**
     * will listen for client connections in a seperate thread, create a new
     * socket and add it to a socket array/vector. so far it is not in a
     * seperate thread, and it only has 1 socket, only 1 person can connect at a
     * time.
     */
    public void connect() throws IOException {
        print("Waiting for connections");
        socket = serverSocket.accept();
        print("Got connection");
        if (handshake()) {
            print("Handshake done. Listening...");
            listenerThread();
        }
    }

    /** Server socket and client sockets does a firm and manly handshake. */
    private boolean handshake() throws IOException {
        PrintWriter out = new PrintWriter(socket.getOutputStream());
        BufferedReader in = new BufferedReader(new InputStreamReader(
                        socket.getInputStream()));

        HashMap<String, String> keys = new HashMap<String, String>();
        String str;

        // Reading client handshake
        print("READ CLIENT HANDSHAKE:");
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
        print("WRITING RESPONSE TO CLIENT:");
        print(response);
        out.write(response);
        out.flush();

        return true;
    }

    private byte[] readBytes(int numOfBytes) throws IOException {
        // print("numOfBytes = " + numOfBytes);
        byte[] b = new byte[numOfBytes];
        socket.getInputStream().read(b);
        return b;
    }

    public void sendMessage(byte[] msg) throws IOException {
        print("Sending to client");
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        BufferedOutputStream os = new BufferedOutputStream(
                        socket.getOutputStream());
        baos.write(SINGLE_FRAME_UNMASKED);
        baos.write(msg.length);
        baos.write(msg);
        baos.flush();
        baos.close();
        // convertAndPrint(baos.toByteArray());
        os.write(baos.toByteArray(), 0, baos.size());
        os.flush();
    }

    public void listenerThread() {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (true) {
                        String msg = reiceveMessage();
                        print("Recieved from client: " + msg);
                        commandStack.add(msg);
                    }
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        t.start();
        print("Started thread used to listen to client messages...");
    }

    public String reiceveMessage() throws IOException {
        byte[] buf = readBytes(2);
        // print("Headers:");
        // convertAndPrint(buf);
        int opcode = buf[0] & 0x0F;
        if (opcode == 8) {
            // Client want to close connection!
            print("Client closed!");
            socket.close();
            System.exit(0);
            return null;
        } else {
            final int payloadSize = getSizeOfPayload(buf[1]);
            // print("Payloadsize: " + payloadSize);
            buf = readBytes(MASK_SIZE + payloadSize);
            // print("Payload:");
            // convertAndPrint(buf);
            buf = unMask(Arrays.copyOfRange(buf, 0, 4),
                            Arrays.copyOfRange(buf, 4, buf.length));
            String message = new String(buf);
            return message;
        }
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

    private void convertAndPrint(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(String.format("%02X ", b));
        }
        print(sb.toString());
    }
}
