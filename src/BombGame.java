import java.io.IOException;
import java.util.StringTokenizer;
import java.util.Vector;


public class BombGame extends MMDGServer{
    
    private int maxNumberOfPlayers;
    private Vector<Player> players;
    
    
    public BombGame(int n) throws IOException {
        super();
        maxNumberOfPlayers = n;
        players = new Vector<Player>();

    }
    
    public void handleClientInput(Vector<String> commandStack){
        String cmd = "";
        for(int i = 0; i<commandStack.size(); ++i){
            cmd = commandStack.elementAt(i);
            
            int ind1 = cmd.indexOf("=");//must be a string " ".
            int ind2 = cmd.indexOf(' ');
            String idStr = cmd.substring(ind1+1, ind2);
            print("id = " + idStr);
            int thisID = Integer.parseInt(idStr);
            
            cmd = cmd.substring(ind2+1);
            if (cmd.compareTo("connected") == 0){
                addPlayer(thisID);
            }
            
        }
    }
    
    private void addPlayer(int id){
        players.add(new Player(id));
        if(players.size() == 2){
            print("Starting Game!");
            startGame();
        }
    }
    
    private void startGame(){
        sendToAllClients("start");
    }
    
    
    private class Player{
        public int id;
        public boolean hasTheBomb;
        
        public Player(int id){
            this.id = id;
            this.hasTheBomb = false;
        }
    }
    
    
    public static void main(String[] args) throws IOException{
        BombGame bg = new BombGame(2);
        bg.run();
    }
}
