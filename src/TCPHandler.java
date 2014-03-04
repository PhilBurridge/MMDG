import java.util.Vector;


public class TCPHandler{
    
    public TCPHandler(int tcpPort){
        
    }

    public void sendMessage(String message) {
        System.out.println("message to send via TCP: " + message);
    }
    
    public void sendMessages(Vector<String> commandStack){
        String messages = "";
        for(int i = 0; i<commandStack.size(); ++i){
            messages += commandStack.elementAt(i) + ";";
        }
        sendMessage(messages);
    }
}
