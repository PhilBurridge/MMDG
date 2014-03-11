import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;


/**
 * This is the server that will initiate communication with our clients. When
 * connecting to the HTTP server they will receive the controller .html and .css
 * files along with the websocket javascript.
 */
public class HTTPServer extends ConsolePrinter{

    /** The HTTP server from com.sun.net. module */
    private HttpServer server;

    /**
     * Creates and starts the HTTP server
     * 
     * @param HOST
     * The ip or DNS address
     * @param HTTP_PORT
     * The http server port
     */
    public HTTPServer(String HOST, int HTTP_PORT) {

        try {
            // Creates the address
            InetSocketAddress address = new InetSocketAddress(HOST, HTTP_PORT);
            // initiates the servers with the right host and port.
            server = HttpServer.create(address, 0);
        } catch (IOException e) {
            print("Error: " + e.getMessage());
            e.printStackTrace();
        }
        // assigning a "content handler" to the server. Server needs to be told
        // what content to serve in response to given URL (now just HOST:PORT/)
        server.createContext("/", new MyHandler());
        // creates a default executor
        server.setExecutor(null);
    }

    /**
     * This is the content handler class which implements the HttpHandler
     * interface. This handler must implement the abstract handler method from
     * HttpHandler interface.
     */
    static class MyHandler implements HttpHandler{

        /**
         * Will respond with any file requested if it exist in public directory.
         * Variable name t seems to be a standard on internet.
         */
        public void handle(HttpExchange t) throws IOException {
            String root = "public/";
            // Gets the request from the URL.
            URI uri = t.getRequestURI();
            File file = new File(root + uri.getPath()).getCanonicalFile();
            if (!file.isFile()) {
                // Object does not exist or is not a file: reject with 404
                // error.
                String response = "404 (Not Found)\n";
                t.sendResponseHeaders(404, response.length());
                OutputStream os = t.getResponseBody();
                os.write(response.getBytes());
                os.close();
            } else {
                // Object exists and is a file: accept with response code 200.
                t.sendResponseHeaders(200, 0);
                // Initiate a output stream that will be sent as response
                OutputStream os = t.getResponseBody();
                // Read the loaded file
                FileInputStream fs = new FileInputStream(file);
                // the buffer into which the data is read
                final byte[] buffer = new byte[0x10000];
                int count = 0;
                // read from file and put into buffer. The read method returns
                // the total number of bytes read or -1 if there is no more data
                // because the end of the stream has been reached.
                while ((count = fs.read(buffer)) >= 0) {
                    // Parameters: Array of characters, offset from which to
                    // start writing, number of characters to write.
                    os.write(buffer, 0, count);
                }
                // Close and send response.
                fs.close();
                os.close();
            }
        }

        // Good tutorials that i've used:
        // http://www.microhowto.info/howto/serve_web_pages_using_an_embedded_http_server_in_java.html
        // <- framför allt
        // http://stackoverflow.com/questions/3732109/simple-http-server-in-java-using-only-java-se-api

        /*
         * There are many alternative ways to do this, and i'm not sure if this
         * is the best solution, looking into servlets might be a good idea.
         * jetty is recommended by many.
         */
    }

    // THis function is probably not needed. http is request-respond based, not
    // an open connection. as long as server is running, anyone can
    // connect(/send requests).
    // if i understand it correctly that is.
    public void listenForNewConnections() {
        // Starts listening to connections
        print("Waiting for connections ... ");
        server.start();
    }

}
