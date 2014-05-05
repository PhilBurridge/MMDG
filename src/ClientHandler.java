import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Arrays;

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
class ClientHandler extends ConsolePrinter implements Runnable{
    /**
     * An instance of the client socket. With this we can read and write to
     * the client
     */
    private Socket clientSocket;

    /** The ID of the client */
    int id;

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
            // Client want to close connection!
            stop();
            return null;
        } else {
            final int payloadSize = WebSocketServer.getSizeOfPayload(buf[1]);
            if (payloadSize == -128) {
                stop();
                return "disconnected";
            }
            buf = readBytes(WebSocketServer.MASK_SIZE + payloadSize);
            // print("Payload:");
            // convertAndPrint(buf);
            buf = WebSocketServer.unMask(Arrays.copyOfRange(buf, 0, 4),
                            Arrays.copyOfRange(buf, 4, buf.length));
            String message = new String(buf);
            return message;
        }
    }

    /**
     * Creates a thread for listening for client messages.
     */
    @Override
    public void run() {
        try {
            while (!Thread.interrupted()) {
                String msg = reiceveMessage();
                print("Recieved from client " + id + ": " + msg);
                WebSocketServer.addCommand("id=" + id + MMDGServer.ARG_DELIMITER + msg);
            }
            print("The listening thread of clientHandler " + id
                            + " is done");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        //print("Started thread used to listen to client messages...");
    }
    

    public void sendMessage(byte[] msg) throws IOException {
        print("Sending to client");
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        BufferedOutputStream os = new BufferedOutputStream(
                        clientSocket.getOutputStream());
        baos.write(WebSocketServer.SINGLE_FRAME_UNMASKED);
        baos.write(msg.length);
        baos.write(msg);
        baos.flush();
        baos.close();
        // convertAndPrint(baos.toByteArray());
        os.write(baos.toByteArray(), 0, baos.size());
        os.flush();
    }
}