#include "robberCop.h"


RobberCop::RobberCop(sgct::Engine * gEngine):
Core(gEngine) {
    scene = new Scene();
    std::cout << "constructing RobberCop" << std::endl;
};

void RobberCop::init(){
    scene->init();
}

// Overiding method from core
void RobberCop::process(int id, std::string var, std::string val) {
    std::cout << "overriding Core process ..." << std::endl;
    Core::process(id,var,val);
    std::cout << "with ..." << std::endl;
    int action;

    if(strcmp(var.c_str(), "btn")){
        int action = atoi(var.substr(3,1).c_str());
        std::cout << "var.substr: " << var.substr(3,1) << std::endl;
        std::cout << "action: " << action << std::endl;
    }

    bool value;
    debug
    if(strcmp(val.c_str(), "pressed")){
        debug
        value = true;
    } else {
        debug
        value = false;
    }

    if(id == 0 || id >= scene->player_vec.size()) {
        debug
        Player *p = new Player(glm::vec2(0.0f, 0.0f), false);
        scene->addPlayer(p);
        debug
    }
    debug
    scene->player_vec.at(id)->setMoveDirection(action, value);
    debug
}

void RobberCop::draw() const {
    scene->update();
    scene->draw();
}


// id=0 var=btn1 val=pressed;