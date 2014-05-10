import java.io.*;
import java.security.NoSuchAlgorithmException;
import mmdg_server.MMDGServer;


public class RunMMDGServer{

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException, InterruptedException, NoSuchAlgorithmException {
        
        int httpPort = 1337;
        int wsPort   = 1338;
        int appPort  = 20501;
        
        MMDGServer mmdgServer = new MMDGServer(httpPort, wsPort, appPort);
        mmdgServer.setUnloadsPerSecond(1000);
        
        mmdgServer.run();
    }
}
