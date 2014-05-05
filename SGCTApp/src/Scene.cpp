#include "Scene.h"



/*Scene::Scene(sgct::Engine * gEngine):
RobberCop(gEngine) {};*/
/*
Scene::Scene(){
    //std::cout << "SIZE: " << player_vec->size() << std::endl;
    //std::cout << "Adding test player: " << std::endl;
    //player_vec = new std::vector<Player *>();
    //player_vec->push_back(new Player(glm::vec2(0.0f, 0.0f), false));

    //std::cout << "SIZE: " << player_vec->size() << std::endl;


}*/

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
    debug
}

void Scene::draw() {
    if(player_vec.size() > 0) {
        for (std::vector<Player *>::iterator it = player_vec.begin() ; it != player_vec.end(); ++it) {
            (*it)->draw((*it)->getPosition());
            //std::cout << "drawloop" << std::endl;
        }
    }
}

void Scene::init() {
    //drawableobject->init();    
}

