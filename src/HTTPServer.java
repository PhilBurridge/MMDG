import java.io.File;
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
        
        private void handleGetMethod(HttpExchange httpExchange) throws IOException {

            File file = new File(this.rootPath + this.fileRef).getCanonicalFile();

        /*    if (!file.isFile()) {
                this.handleError(httpExchange, 404);
            } else if (!file.getPath().startsWith(this.rootPath.replace('/', '\\'))) { // windows work with anti-slash!
                // Suspected path traversal attack.
                System.out.println(file.getPath());
                this.handleError(httpExchange, 403);
            } else {*/
                //Send the document.


                httpExchange.sendResponseHeaders(200, file.length());       
                System.out.println("file length : "+ file.length() + " bytes.");


                OutputStream os = httpExchange.getResponseBody();

                FileInputStream fs = new FileInputStream(file);

                final byte[] buffer = new byte[1024];
                int count = 0;
                while ((count = fs.read(buffer)) >= 0) {
                    os.write(buffer, 0, count);
                }
                os.flush();
                fs.close();
                os.close();
            }
       /* public void handle(HttpExchange t) throws IOException {
            String response = "This is the response";
            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
        }*/
    }
    
    
    public void listenForNewConnections(){
        
    }

}

