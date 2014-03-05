import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

public class HTTPServer {

        HttpServer server;


        
        public HTTPServer(int HTTP_PORT){
            
            try {
                server = HttpServer.create(new InetSocketAddress(HTTP_PORT), 0);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            server.createContext("/test", new MyHandler());
            server.setExecutor(null); // creates a default executor
            server.start();
        }
        
        
    static class MyHandler implements HttpHandler {
        public void handle(HttpExchange t) throws IOException {
            String response = "This is the response";
            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
        }
    }

}