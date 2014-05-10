import java.io.*;
import java.security.NoSuchAlgorithmException;
import mmdg_server.MMDGServer;


public class RunMMDGServer{

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException, InterruptedException, NoSuchAlgorithmException {
        MMDGServer mmdgServer = new MMDGServer();
        mmdgServer.run();
    }
}
