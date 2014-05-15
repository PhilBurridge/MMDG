#include "Scene.h"


// Scene constructor
Scene::Scene(){
    background = new DrawableObject("mmdg", 2.0f, 1.0f);
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

        
void Scene::addPlayer(int id, Player *p) {
    // Set cop texture and start cop timer
    if(p->isCop())
        p->switchToCop();

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

// Gets the amount of currently connected players
unsigned int Scene::getNumberOfPlayers() {
    return players.size();
}

void Scene::draw() {
    //Draw Background image
    background->draw(0.0f, 0.0f, -0.01f);

    for(std::map<int, Player *>::iterator it = players.begin(); it != players.end(); it++) {
        std::pair<int, Player *> pair = *it;
        
        // Disable depth test for the alpha blending to draw correct when players collide
        glDisable(GL_DEPTH_TEST);
        pair.second->draw();
        printPlayerName(pair.first);
    }
}

// Prints the id of a player above it's canvas,
// later change this to the players name, i.e a string
void Scene::printPlayerName(int id) {

// pixel x: 1280 / 2 = 640
// pixel y: 720 / 2 = 360
// Om någon vill fixa 3d -> pixel-koordinater bättre, then pls do!

    // Convert int to string
    std::stringstream ss;
    ss << id;
    std::string str_id = ss.str();

    // Print the id to the screen
    sgct_text::print(sgct_text::FontManager::instance()->getFont
        ("SGCTFont", 14 ), 
        (players.at(id)->getPosition().x * 360) + 640, 
        (players.at(id)->getPosition().y * (640/1.778)) + 420, 
        str_id.c_str());
}
