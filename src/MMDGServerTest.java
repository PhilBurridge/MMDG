import java.io.*;
import java.security.NoSuchAlgorithmException;


public class MMDGServerTest{

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException, InterruptedException, NoSuchAlgorithmException {
        MMDGServer mmdgServer = new MMDGServer();
        mmdgServer.run();

    }

    /*
     * public static void main(String[] args) { MMDGServer mmdgServer = new
     * MMDGServer(); mmdgServer.setUnloadsPerSecond(10); mmdgServer.run(); }
     */
    

    /*public static void main(String[] args) throws IOException,
                     {
        WebSocketServer j = new WebSocketServer();
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
            System.out.println("Write something to the client!");
            j.sendMessage(br.readLine().getBytes());
        }
    }*/
    
}
