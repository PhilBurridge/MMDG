import java.util.Vector;


public class TCPHandler extends ConsolePrinter{ 

    public TCPHandler(int tcpPort) {

    }

    public void sendMessage(String message) {
        print("sending TCP message: \"" + message + "\" ");
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
