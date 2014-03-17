import java.io.*;
import java.net.*;
import java.util.Vector;


public class TCPHandler extends ConsolePrinter{ 

    public TCPHandler(int tcpPort) {

    }

    public void sendMessage(String message) {
        print("sending TCP message: \"" + message + "\" ");
    }

    public void sendMessages(Vector<String> commandStack) {
        if (commandStack.size() == 0) {
            print("Emtpy message.");
            return;
        }

        String messages = "";
        for (int i = 0; i < commandStack.size(); ++i) {
            messages += commandStack.elementAt(i) + ";";
        }
        sendMessage(messages);
    }
}

/*
String sentence;
String modifiedSentence;
BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));

Socket clientSocket = new Socket("localhost", 20501);
DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

sentence = inFromUser.readLine();
outToServer.writeBytes(sentence + '\n');
modifiedSentence = inFromServer.readLine();
System.out.println(modifiedSentence);
//clientSocket.close();*/