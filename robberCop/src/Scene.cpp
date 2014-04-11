#include "Scene.h"
#include "Player.h"
#include "sgct.h"
#include "DrawableObject.h"

void update() {
    updatePositions();
    checkCollisions();
}

void updatePositions() {
    for(std::vector<Player> it = player_vec; it != player_vec.end(); it++) { 
        (*it)->movePlayer();
    }
}

void checkCollisions() {
    for(std::vector<Player> itCop = player_vec; itCop != player_vec.end(); itCop++) { // jag hade ingen bättre fantasi än 
        if((*itCop)->isCop()) {
            for(std::vector<Player> itRob = player_vec; itRob != player_vec.end(); itRob++) {
                if(!(*itRob)->isCop() && 
                    lenght((*isCop)->position - (*isRob)->position) > ((*isCop)->size + (*isRob)->size)) { // oskäer på length
                    // Do somthing when collision happens. KILL THA ROBBBA
                }
            }
        }
    }
}

void draw() {
    
}


