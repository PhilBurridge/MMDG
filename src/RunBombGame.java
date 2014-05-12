

import java.io.IOException;
import bomb_game.BombGame;

public class RunBombGame{
    public static void main(String[] args) throws IOException{
        int httpPort = 1337;
        int wsPort   = 1338;
        int appPort  = 20501;
        
        BombGame bg = new BombGame(httpPort, wsPort, appPort,4);
        bg.setExplosionInterval(5,20);
        bg.run();
    }
}
