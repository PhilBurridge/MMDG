import java.io.*;
import java.net.URL;

import java.security.NoSuchAlgorithmException;
import mmdg_server.MMDGServer;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class RunMMDGServer extends JFrame implements ActionListener{
	
    private static final long serialVersionUID = 1L;
    
    private JPanel panelMain, panelText;
	private JLabel lblIp, lblHttp, lblWs, lblApp, lblAppIP;
	private JButton btnStart,btnTerminate;
	private JTextField txtIp, txtHttp, txtWs, txtApp, txtAddress, txtAppIP; 
	private MMDGServer mmdgServer;
	
	 public RunMMDGServer() 
	 {
		 
	 	panelMain = new JPanel();
	 	panelText = new JPanel();
	 	
	 	lblIp = new JLabel("IP: ", JLabel.RIGHT);
	 	lblHttp = new JLabel("HTTP port: ", JLabel.RIGHT);
	 	lblWs = new JLabel("ws port: ", JLabel.RIGHT);
	 	lblApp = new JLabel("App port: ", JLabel.RIGHT);
	 	lblAppIP = new JLabel("Application IP: ", JLabel.RIGHT);
	 	
	 	
	 	txtIp = new JTextField("");
	 	txtIp.setEditable(false);
	 	txtHttp = new JTextField("1337");
	 	txtWs = new JTextField("1338");
	 	txtApp = new JTextField("20500");
	 	txtAddress=  new JTextField(" ",JTextField.CENTER);
	 	txtAddress.setEditable(false);
	 	txtAppIP = new JTextField("localhost");
	 	
	 	btnStart = new JButton("Starta servern");
	 	btnTerminate = new JButton("Avsluta servern");
	 	
		btnStart.addActionListener(this);
		btnTerminate.addActionListener(this);
		 
		panelMain.setLayout(new GridLayout(6,2));
		panelMain.add(lblIp);
 		panelMain.add(txtIp);
 		panelMain.add(lblHttp);
 		panelMain.add(txtHttp);
 		panelMain.add(lblWs);
 		panelMain.add(txtWs);
 		panelMain.add(lblApp);
 		panelMain.add(txtApp);
 		panelMain.add(lblAppIP);
        panelMain.add(txtAppIP);
 		panelMain.add(btnStart);
 		panelMain.add(btnTerminate);
 		panelText.setLayout(new BorderLayout());
 		panelText.add(txtAddress,BorderLayout.CENTER);
	 		
	 	Container c = getContentPane();
 		c.setLayout(new BorderLayout());
	 	c.add(panelMain, BorderLayout.CENTER);
	 	c.add(panelText, BorderLayout.SOUTH);

	 		
        setTitle("MMDG Server");
        setSize(450,150);
        setVisible(true);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
	 }
	 
	 public void start(int http, int ws, int app, String appIP) throws IOException, InterruptedException, NoSuchAlgorithmException {
	     
		 mmdgServer = new MMDGServer(http, ws, app, appIP);
         mmdgServer.setUnloadsPerSecond(1000);
         mmdgServer.runServer();
	 }
	
	public void actionPerformed(ActionEvent e) 
    {
    	if(e.getSource() == btnStart)
    	{
    		
    		int httpPort = Integer.parseInt(txtHttp.getText());
    		txtHttp.setEditable(false);
            int wsPort   = Integer.parseInt(txtWs.getText());
            txtWs.setEditable(false);
            int appPort  = Integer.parseInt(txtApp.getText());
            System.out.println(wsPort);
            txtApp.setEditable(false);
            String appIP=txtAppIP.getText();
            txtAppIP.setEditable(false);
            
            //H€MTA IP
            // This is probably a wierd way to get the IP address...
            String ipAdress = "";
            try {
                URL whatismyip = new URL("http://checkip.amazonaws.com/");
                BufferedReader in = new BufferedReader(new InputStreamReader(
                                whatismyip.openStream()));

                ipAdress = in.readLine(); // you return the IP as a String
            } catch (Exception eIp) {
            	ipAdress = "Could not find IP";
                eIp.printStackTrace();
            }
            txtIp.setText(ipAdress);
            
            try {
				start(httpPort, wsPort, appPort, appIP);
			} catch (IOException e1) {
				e1.printStackTrace();
			} catch (NoSuchAlgorithmException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
            txtAddress.setText("http://" + ipAdress + ":" + httpPort + "/robbercop.html");
    	}
    	
    	if(e.getSource() == btnTerminate)
    	{
    		mmdgServer.stopServer();
    		txtAddress.setText("Server Stopped");
    	}
    	
    }

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException, InterruptedException, NoSuchAlgorithmException {

    	/*int httpPort = 1337;
        int wsPort   = 1338;
        int appPort  = 20500;*/

        
        RunMMDGServer frame = new RunMMDGServer();
        //MMDGServer mmdgServer = new MMDGServer(httpPort, wsPort, appPort);
        //mmdgServer.setUnloadsPerSecond(1000);
        
        //mmdgServer.run();
    }
}
