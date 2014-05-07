#include "Scene.h"

// Scene constructor
Scene::Scene(){
    background = new DrawableObject("mmdg", 2.0f, 1.0f);
}

// Updates all the required stuff for players before drawing
void Scene::update(float dt) {
    updatePositions(dt);
    checkCollisions();
}

// Updates the positions of all players with a specific step
void Scene::updatePositions(float dt) {
    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        pair.second->movePlayer(dt);
    }
}

void Scene::checkCollisions() {
  
    //With std::map
    Player * p1;
    Player * p2;

    for(std::map<int, Player *>::iterator itCop = players.begin(); itCop != players.end(); ++itCop) {
        p1 = (*itCop).second;
        if(!p1->isCop()) 
            continue; //om p1 inte är polis, fuck it. leta vidare
        //Om vi har kommit hit så vet vi att p1 är en polis
        for(std::map<int, Player *>::iterator itRob = players.begin(); itRob != players.end(); ++itRob) {
            if((*itRob).first == (*itCop).first)
                continue;

            p2 = (*itRob).second;

            if(p2->isCop())
                continue; //om p2 var polis, fuck it. leta vidare. 

            //Om vi har kommit hit så vet vi att p1 är en polis och p2 är en Robber
            //Nu kollar vi om de kolliderar med varandra. De är sfärer
            if(glm::length(p1->getPosition() - p2->getPosition()) < (p1->getSize() + p2->getSize())) { // oskäer på length
                // Do somthing when collision happens. KILL THA ROBBBA
                p2->switchToCop();
                std::cout << "****************************************" << std::endl;
                std::cout << "collision between player " << (*itRob).first << " and " << (*itCop).first << std::endl;
                std::cout << "****************************************" << std::endl;
            }
        }
    }

}

        
void Scene::addPlayer(int id, Player *p) {
    players.insert(std::pair<int, Player *>(id, p));
}

bool Scene::removePlayer(int id){
    std::map<int, Player *>::iterator it = players.find(id);
    
    if(it == players.end())
        return false;

    players.erase(it);
    return true;
}

Player * Scene::getPlayer(int id){
    return players[id];
}

void Scene::draw() {
    //Draw Background image
    background->draw(0.0f, 0.0f, -0.01f);

    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        
        // If the player is a cop, swap the texture (rob texture is standard)
        if(pair.second->isCop()) {
            pair.second->switchToCop();
        }
        // Disable depth test for the alpha blending to draw correct when players collide
        glDisable(GL_DEPTH_TEST);
        pair.second->draw();
    }
}


