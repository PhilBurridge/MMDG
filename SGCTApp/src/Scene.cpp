#include "Scene.h"



/*Scene::Scene(sgct::Engine * gEngine):
RobberCop(gEngine) {};*/

Scene::Scene(){
    background = new DrawableObject();
}

void Scene::update() {
    updatePositions();
    checkCollisions();
}

void Scene::updatePositions() {
    for(std::vector<Player *>::iterator it = player_vec.begin(); it != player_vec.end(); it++) {
        (*it)->movePlayer();
    }
}

void Scene::checkCollisions() {
    for(std::vector<Player *>::iterator itCop = player_vec.begin(); itCop != player_vec.end(); itCop++) { // jag hade ingen bättre fantasi än 
        if((*itCop)->isCop()) {
            for(std::vector<Player *>::iterator itRob = player_vec.begin(); itRob != player_vec.end(); itRob++) {
                if(!(*itRob)->isCop() && 
                    glm::length((*itCop)->getPosition() - (*itRob)->getPosition()) > ((*itCop)->getSize() + (*itRob)->getSize())) { // oskäer på length
                    // Do somthing when collision happens. KILL THA ROBBBA
                    std::cout << "collision between player " << std::endl;
                }
            }
        }
    }
}

        
void Scene::addPlayer(Player *p) {
    debug
    player_vec.push_back(p);
    //player_vec.resize(player_vec.size()+1, p);
    std::cout << "player_vec.size(): " << player_vec.size() << std::endl;
    //player_vec.resize(player_vec.size()+1, p);
    debug
}

void Scene::draw() {
    //if(player_vec.size() > 0) {
        //debug
    //std::cout << "player_vec.size(): " << player_vec.size() << std::endl;
        for (std::vector<Player *>::iterator it = player_vec.begin() ; it != player_vec.end(); ++it) {
            //debug
            (*it)->draw();
            //std::cout << "drawloop" << std::endl;
        }
    //}
}

void Scene::init() {
    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        backgroundhandle, "Tex", "./textures/box.png", true);
}

