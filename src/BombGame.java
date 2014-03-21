import java.io.IOException;
import java.util.Random;
import java.util.TimerTask;
import java.util.Vector;
import java.util.Timer;


public class BombGame extends MMDGServer{
    
    private boolean gameOn;
    private int maxNumberOfPlayers;
    private Vector<Player> players;
    private Timer timer;
    private int minTimeBeforeExplosion;
    private int maxTimeBeforeExplosion; 
    
    public BombGame(int n) throws IOException {
        super();
        maxNumberOfPlayers = n;
        players = new Vector<Player>();
        gameOn = false;
    }
    
    public void handleClientInput(Vector<String> commandStack){
        String cmd = "";
        for(int i = 0; i<commandStack.size(); ++i){
            cmd = commandStack.elementAt(i);
            print("incoming command: " + cmd);
            
            //Figure out which client whos talking
            int ind1 = cmd.indexOf("=");//must be a string " ".
            int ind2 = cmd.indexOf(' ');
            String idStr = cmd.substring(ind1+1, ind2);
            int thisID = Integer.parseInt(idStr);
            int thisIndex = idToIndex(thisID);
            
            cmd = cmd.substring(ind2+1);
            if (cmd.compareTo("connected") == 0){
                print("player " + thisID + " connected");
                addPlayer(thisID);
            }
            
            if(cmd.length() > 10 && cmd.substring(0, 10).compareTo("hasTheBomb") == 0){
                boolean hasTheBomb = Boolean.parseBoolean(cmd.substring(11));
                print("id " + thisID + " asserts hasTheBomb=" + hasTheBomb + ". Java says hasTheBomb=" + players.elementAt(thisIndex).hasTheBomb);
                if(players.elementAt(thisIndex).hasTheBomb != hasTheBomb){
                    print("BOMB EXPLODED IN THE AIR!!!!!!!!!!!!!!!");
                    /*
                    sendToAllClients("you win");
                    sleep(1000);
                    
                    int seconds = 5;
                    sendToAllClients("countdown="+seconds);
                    sleep(seconds*1000);
                    startGame();
                    */
                }
            }
            
            if(!gameOn)
                return;
            if (cmd.compareTo("not me!") == 0){
                print("player " + thisID + " threw the bomb");
                players.elementAt(thisIndex).hasTheBomb = false;
                throwBombRandomly();
            }
            if (cmd.compareTo("damn it") == 0){
                gameOn = false;
                print("player " + thisID + " lost");
                sendToAllClientsExcept(thisID, "you win");
                sleep(1000);
                
                int seconds = 10;
                sendToAllClientsExcept(thisID, "countdown="+seconds);
                sleep(seconds*1000);
                startGame();
            }
        }
    }
    
    private void setExplosionInterval(int min, int max){
        minTimeBeforeExplosion = min;
        maxTimeBeforeExplosion = max;
    }
    
    private void addPlayer(int id){
        players.add(new Player(id));
        if(players.size() == maxNumberOfPlayers){
            startGame();
        }
    }
    
    private void startGame(){
        for(int i = 0; i<players.size(); ++i){
            players.elementAt(i).hasTheBomb = false;
        }
        print("Starting Game!");
        
        gameOn = true;
        sendToAllClients("start");
        sleep(1000);
        throwBombRandomly();
        startTicking();
    }
    
    private void startTicking(){
        Random r = new Random();
        int secondsToExplosion = minTimeBeforeExplosion + 
                        r.nextInt(maxTimeBeforeExplosion-minTimeBeforeExplosion);
        timer = new Timer();
        timer.schedule(new TimerTask(){
            @Override
            public void run(){
                bombExplode();
            }
            
        }, secondsToExplosion * 1000);
    }
    
    private void bombExplode(){
        print("bomb exploded");
        for(int i = 0; i<players.size(); ++i)
            print(players.elementAt(i));
        
        sendToAllClients("explode");
    }
    
    private void sendToAllClientsExcept(int id, String msg){
        for(int j = 0; j<players.size(); ++j){
            if (players.elementAt(j).id != id){
                int othersID = players.elementAt(j).id;
                sendToClient(othersID, msg);
            }
        }
    }
    
    private void throwBombRandomly(){
        Random r = new Random();
        int newInd = r.nextInt(players.size());
        int newID = players.elementAt(newInd).id;
        throwBombTo(newID);
    }
    
    private void throwBombTo(int id){
        print("throw bomb to " + id);
        sendToClient(id, "catch");
        int index = idToIndex(id);
        players.elementAt(index).hasTheBomb = true;
    }
    
    private int idToIndex(int id){
        for (int i = 0; i<players.size(); ++i){
            if (players.elementAt(i).id == id)
                return i;
        }
        return -1;
    }
    
    private class Player{
        public int id;
        public boolean hasTheBomb;
        
        public Player(int id){
            this.id = id;
            this.hasTheBomb = false;
        }
        
        public String toString(){
            return "id=" + id + " hasTheBomb=" + hasTheBomb;
        }
    }
    
    private void sleep(int millis){
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    public static void main(String[] args) throws IOException{
        BombGame bg = new BombGame(4);
        bg.setExplosionInterval(5,20);
        bg.run();
    }
}
