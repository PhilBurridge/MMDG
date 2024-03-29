#include "Scene.h"


// Scene constructor
Scene::Scene(){
    background = new DrawableSquare("mmdg", 4.0f, 3.0f);
    bg_sphere = new ModelMesh("sphere", "mmdg");

}

// Gets the map containing all players
const std::map<int, Player *>& Scene::getPlayerMap() const{
    return players;
}

// Sets the positions of all players, used for sharing the positions over the cluster
void Scene::setPlayerPositions(std::vector<glm::vec2> positions){
    if(positions.size() != players.size()){
        
        std::cout << "Error in Scene: Updating all player positions - size must match!" << std::endl;
        std::cout << "positions.size() = " << positions.size() << std::endl;
        std::cout << "players.size() = " << players.size() << std::endl;
        
        return;
    }

    Player * p;
    int positionIndex = 0;
    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        p = pair.second;
        p->setPosition(positions[positionIndex]);
        positionIndex++;
    }
}

// Sets the map containing all players
void Scene::setPlayerMap(std::map<int, Player *> playerMap){
    players = playerMap;
}

// Updates all the required stuff for players before drawing
void Scene::update(float dt) {
    Player * p;
    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        p = pair.second;

        // Updates the positions of all players with a specific step
        p->movePlayer(dt);

        // Make cops to robber if they have caught any robbers in a while
        if(p->isCop() && !p->copTimer()){
            p->switchToRobber();
        }
    }
    checkCollisions();
}


// Checks for collisions between players
void Scene::checkCollisions() {
    Player * p1;
    Player * p2;

    for(std::map<int, Player *>::iterator itCop = players.begin(); itCop != players.end(); ++itCop) {
        p1 = (*itCop).second;

        //if p1 is no cop, fuck it. keep on looking
        if(!p1->isCop()) 
            continue;

        //If we've gotten here, we know p1 is a cop
        //lets see if we can collide with some robbers
        for(std::map<int, Player *>::iterator itRob = players.begin(); itRob != players.end(); ++itRob) {
            p2 = (*itRob).second;

            //if p2 is a cop, fuck it. leta vidare. We want robbers
            if(p2->isCop())
                continue;

            //If we've gotten here, we know p1 is a cop and p2 is a robber
            //Check if there is a collision
            if(glm::length(p1->getPosition() - p2->getPosition()) < (p1->getSize() + p2->getSize())) { 

                // Do somthing when collision happens. KILL THA ROBBBA
                p1->resetCopTimer();
                p2->switchToCop();

                std::cout << "****************************************" << std::endl;
                std::cout << "collision between player " << (*itRob).first << " and " << (*itCop).first << std::endl;
                std::cout << "****************************************" << std::endl;
            }
        }
    }
}

// Adds a player to the game
void Scene::addPlayer(int id, Player *p) {
    // Set cop texture and start cop timer
    if(p->isCop())
        p->switchToCop();

    players.insert(std::pair<int, Player *>(id, p));
}

// Removes a player from the game
bool Scene::removePlayer(int id){
    std::map<int, Player *>::iterator it = players.find(id);
    
    if(it == players.end())
        return false;

    players.erase(it);
    return true;
}

// Gets a player by id
Player * Scene::getPlayer(int id){
    return players[id];
}

// Gets the amount of currently connected players
unsigned int Scene::getNumberOfPlayers() {
    return players.size();
}

// Draws the scene in dome or normal desktop mode
void Scene::draw(bool drawSpherical) {
    if(drawSpherical){ 
        // Draw Dome mode
        glPushMatrix();
            float s = 1.5f;
            glScalef(s,s,s);
            bg_sphere->draw();
        glPopMatrix();

        for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
            std::pair<int, Player *> pair = *it;
            pair.second->drawSpherical();
        }
    }
    else{ 
        //Draw normal desktop mode
        background->draw(0.0f, 0.0f, -0.01f);

        for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
            std::pair<int, Player *> pair = *it;
            pair.second->draw();
        }
    }
}
