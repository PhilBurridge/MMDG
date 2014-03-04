import java.util.Vector;

/** 
 *
 */
public class WebSocketServer{
    private Vector<String> commandStack;
    VirtualClient cs;
    
    public WebSocketServer(int websocketPort){
        commandStack = new Vector<String>();
    }
    
    public void listenToWebSocketMessages(){
        cs = new VirtualClient(this,1);
        cs = new VirtualClient(this,2);
    }
    
    public void addCommand(String command){
        commandStack.add(command);
    }
    
    public Vector<String> getCommandStack(){
        return commandStack;
    }
    
    public void clearCommandStack(){
        commandStack.clear();
    }
}
