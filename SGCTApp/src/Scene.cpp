#include "Scene.h"


Scene::Scene(){
    background = new DrawableObject("mmdg", 2.0f, 1.0f);
}

void Scene::init() {
    
}


void Scene::update(float dt) {
    updatePositions(dt);
    checkCollisions();
}

void Scene::updatePositions(float dt) {
    //for(std::vector<Player *>::iterator it = player_vec.begin(); it != player_vec.end(); it++) {
    //    (*it)->movePlayer();
    //}
    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        pair.second->movePlayer(dt);
    }
}

void Scene::checkCollisions() {
    //With std::vector
    /*
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
    */

    //With std::map
    Player * p1;
    Player * p2; 
    for(std::map<int, Player *>::iterator itCop = players.begin(); itCop != players.end(); itCop++) {

        p1 = (*itCop).second;
        if(!p1->isCop()) 
            continue; //om p1 inte är polis, fuck it. leta vidare

        //Om vi har kommit hit så vet vi att p1 är en polis
        for(std::map<int, Player *>::iterator itRob = itCop; itRob != players.end(); itRob++) {

            p2 = (*itRob).second;
            if(p2->isCop())
                continue; //om p2 var polis, fuck it. leta vidare. 

            //Om vi har kommit hit så vet vi att p1 är en polis och p2 är en Robber
            //Nu kollar vi om de kolliderar med varandra. De är sfärer

            if(glm::length(p1->getPosition() - p2->getPosition()) > (p1->getSize() + p2->getSize())) { // oskäer på length
                // Do somthing when collision happens. KILL THA ROBBBA
                std::cout << "collision between player " << std::endl;
            }
        }
    }

}

        
void Scene::addPlayer(int id, Player *p) {
    //With std::vector
    //player_vec.push_back(p);
    //std::cout << "player_vec.size(): " << player_vec.size() << std::endl;


    //With std::map
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

    //With std::map
    return players[id];
}

void Scene::draw() {
    //Draw Background image
    background->draw(0.0f, 0.0f, -0.01f);

    /*for (std::vector<Player *>::iterator it = player_vec.begin() ; it != player_vec.end(); ++it) {
        (*it)->draw();
    }*/
    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        
        // If the player is a cop, swap the texture (rob texture is standard)
        if(pair.second->isCop()) {
            pair.second->switchToCop();
        }

        pair.second->draw();
    }
}


