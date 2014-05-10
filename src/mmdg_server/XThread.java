package mmdg_server;
/**
 * This Class provides a safe sleep function that can be used
 * in all the classes. Just call XThread.delay(milliseconds)
 * and it will block the thread for that many milliseconds.
 * It will also return true or false if the thread was 
 * "interrupted" mid sleep. 
 */
public class XThread extends Thread{
    public static boolean delay(long millis) {
        if (interrupted())
            return false;
        try {
            sleep(millis);
        }
        catch(InterruptedException e) {
            return false;
        }
        return true;
        
    }
    
}
