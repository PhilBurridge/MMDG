
package bomb_game;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Random;
import java.util.TimerTask;
import java.util.Timer;
import mmdg_server.MMDGServer;


public class BombGame extends MMDGServer{

    private boolean gameOn;
    private int maxNumberOfPlayers;
    private HashMap<Integer, Player> players;
    private Timer timer;
    private int minTimeBeforeExplosion;
    private int maxTimeBeforeExplosion;

    public BombGame(int httpPort, int wsPort, int appPort, int n)
                    throws IOException {
        super(httpPort, wsPort, appPort);
        maxNumberOfPlayers = n;
        players = new HashMap<Integer, Player>();
        gameOn = false;
    }

    protected void processMessagesFromClients(ArrayList<String> commandStack) {
        
        
        String cmd = "";
        for (int i = 0; i < commandStack.size(); ++i) {
            cmd = commandStack.get(i);
            print("incoming command: " + cmd);

            // Figure out which client whos talking
            int ind1 = cmd.indexOf("=");// must be a string " ".
            int ind2 = cmd.indexOf(' ');
            String idStr = cmd.substring(ind1 + 1, ind2);
            int thisID = Integer.parseInt(idStr);
            print("this id = " + thisID);

            cmd = cmd.substring(ind2 + 1);
            print("cmd = " + cmd);
            
            if (cmd.compareTo("connected") == 0) {
                print("player " + thisID + " connected");
                addPlayer(thisID);
            }
            
            if (cmd.startsWith("var=disconnected")) {
                print("Disconnecting player " + thisID);
                players.remove(thisID);
            }

            if (cmd.length() > 10 && cmd.startsWith("hasTheBomb")) {
                boolean hasTheBomb = Boolean.parseBoolean(cmd.substring(11));
                print("id " + thisID + " asserts hasTheBomb=" + hasTheBomb
                                + ". Java says hasTheBomb="
                                + players.get(thisID).hasTheBomb);
                if (players.get(thisID).hasTheBomb != hasTheBomb) {
                    print("BOMB EXPLODED IN THE AIR!!!!!!!!!!!!!!!");
                    /*
                     * sendToAllClients("you win"); sleep(1000);
                     * 
                     * int seconds = 5; sendToAllClients("countdown="+seconds);
                     * sleep(seconds*1000); startGame();
                     */
                }
            }

            if (!gameOn) return;
            if (cmd.compareTo("not me!") == 0) {
                print("player " + thisID + " threw the bomb");
                players.get(thisID).hasTheBomb = false;
                throwBombRandomly();
            } else if (cmd.compareTo("damn it") == 0) {
                gameOn = false;
                print("player " + thisID + " lost");
                sendToAllClientsExcept(thisID, "you win");

                int seconds = 10;
                delayedSendToAllClientsExcept(thisID, "countdown=" + seconds, 1000);
                delayedStartGame(seconds*1000);
            }
        }
    }

    public void setExplosionInterval(int min, int max) {
        minTimeBeforeExplosion = min;
        maxTimeBeforeExplosion = max;
    }

    private void addPlayer(int id) {
        players.put(id, new Player());
        print("players.size() = " + players.size());
        print("maxNumberOfPlayers = " + maxNumberOfPlayers);
        if (players.size() == maxNumberOfPlayers) {
            startGame();
        }
    }

    private void startGame() {
        Iterator<Entry<Integer, Player>> it = players.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Integer, Player> pairs = (Map.Entry<Integer, Player>) it
                            .next();
            pairs.getValue().hasTheBomb = false;
        }

        print("Starting Game!");

        gameOn = true;
        sendToAllClients("start");
        sleep(1000);
        throwBombRandomly();
        startTicking();
    }
    
    private void delayedStartGame(final int delayMillis){
        Thread t = new Thread(new Runnable(){
            public void run(){
                sleep(delayMillis);
                startGame();
            }
        });
        t.run();
    }

    private void startTicking() {
        Random r = new Random();
        int secondsToExplosion = minTimeBeforeExplosion
                        + r.nextInt(maxTimeBeforeExplosion
                                        - minTimeBeforeExplosion);
        timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                bombExplode();
            }

        }, secondsToExplosion * 1000);
    }

    private void bombExplode() {
        print("bomb exploded");
        Iterator<Entry<Integer, Player>> it = players.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Integer, Player> pairs = (Map.Entry<Integer, Player>) it
                            .next();
            print(pairs.getValue());
        }

        sendToAllClients("explode");
    }

    private void sendToAllClientsExcept(int id, String msg) {
        Iterator<Entry<Integer, Player>> it = players.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Integer, Player> pairs = (Map.Entry<Integer, Player>) it
                            .next();
            if (pairs.getKey() != id) {
                sendToClient(pairs.getKey(), msg);
            }
        }
    }
    
    private void delayedSendToAllClientsExcept(final int id, final String msg, final int delayMillis){
        Thread t = new Thread(new Runnable(){
            public void run(){
                sleep(delayMillis);
                sendToAllClientsExcept(id, msg);
            }
        });
        t.run();
    }

    private void throwBombRandomly() {
        Random r = new Random();
        int playerNumber = r.nextInt(players.size());

        Iterator<Entry<Integer, Player>> it = players.entrySet().iterator();
        Map.Entry<Integer, Player> pairs;
        int i = 0;
        do {
            pairs = (Map.Entry<Integer, Player>) it.next();
            i++;
        } while (i < playerNumber);

        throwBombTo(pairs.getKey());
    }

    private void throwBombTo(int id) {
        print("throw bomb to " + id);
        sendToClient(id, "catch");
        players.get(id).hasTheBomb = true;
    }

    private class Player{
        public boolean hasTheBomb;

        public Player() {
            this.hasTheBomb = false;
        }

        public String toString() {
            return "hasTheBomb=" + hasTheBomb;
        }
    }
    
    

    private void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
