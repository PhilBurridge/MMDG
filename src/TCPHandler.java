import java.util.Vector;


public class TCPHandler{

    public TCPHandler(int tcpPort) {

    }

    public void sendMessage(String message) {
        System.out.print("\nTCP-message: \"" + message + "\" ");
    }

    public void sendMessages(Vector<String> commandStack) {
        if (commandStack.size() == 0) {
            System.out.print('.');
            return;
        }

        String messages = "";
        for (int i = 0; i < commandStack.size(); ++i) {
            messages += commandStack.elementAt(i) + ";";
        }
        sendMessage(messages);
    }
}
