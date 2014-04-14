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
    for(std::vector<Player> itCop = player_vec.begin(); itCop != player_vec.end(); itCop++) { // jag hade ingen bättre fantasi än 
        if((*itCop)->isCop()) {
            for(std::vector<Player> itRob = player_vec.begin(); itRob != player_vec.end(); itRob++) {
                if(!(*itRob)->isCop() && 
                    lenght((*isCop)->position - (*isRob)->position) > ((*isCop)->size + (*isRob)->size)) { // oskäer på length
                    // Do somthing when collision happens. KILL THA ROBBBA
                    std::cout << "collision between player " << std::endl;
                }
            }
        }
    }
}

void draw() {
    // Borde bara loopa igenom alla player och rita ut dem mha draw i drawableObject
}


