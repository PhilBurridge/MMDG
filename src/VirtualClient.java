import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.*;
import javax.swing.*;


/**
 * This class is used to test the mmdgServer behavior. Will be replaces with
 * websocket server implementation in WebSocketServer.java.
 */
public class VirtualClient extends JFrame implements ActionListener{
    private static final long serialVersionUID = 1L;

    private JButton btn1;
    private JButton btn2;
    private WebSocketServer webSocketServer;
    private TCPHandler tcphandler;
    private int id;

    public VirtualClient(WebSocketServer wss, int id) {
        this.id = id;
        webSocketServer = wss;

        btn1 = new JButton("Button 1");
        btn1.addActionListener(this);

        btn2 = new JButton("Button 2");
        btn2.addActionListener(this);

        Container c = getContentPane();
        c.setLayout(new BorderLayout());
        c.add(btn1, BorderLayout.WEST);
        c.add(btn2, BorderLayout.EAST);

        setSize(750, 300);
        setTitle("Encrypter 1.0");
        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == btn1) {
            String msg = "value=1" + "\r\n";
            webSocketServer.addCommand("client=" + id + ",button=1,pressed");
            tcphandler.sendMessage(msg);
        }

        if (e.getSource() == btn2) {
            webSocketServer.addCommand("client=" + id + ",button=2,pressed");
        }
    }
}
