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
    std::cout << "INPUT: " << std::endl;
    Core::process(id,var,val);

    std::cout << "HANDELING INPUT" << std::endl;

    if(var == "connected"){
        std::cout << "adding new player" << std::endl;
        debug
        Player *p = new Player(glm::vec2(1.0f, 0.0f), false);
        debug
        sgct::TextureManager::instance()->loadTexure(
            p->textureHandle, "player0", "./textures/" + p->texture, true);

        debug
        scene->addPlayer(id, p);

        return;
    }

    if(scene->getPlayer(id) == NULL){
        std::cout << "WARNING! TRYING TO ACCESS NULL POINTER" << std::endl;
        return;
    }

    if(var.substr(0,3) == "btn"){
        int btnNumber = atoi(var.substr(3,1).c_str());
        bool pressed = (val == "1");
        std::cout << "btn number: " << btnNumber << std::endl;
        std::cout << "btn is pressed: " << pressed << std::endl;
        scene->getPlayer(id)->setMoveDirection(btnNumber, pressed);
    }
}

void RobberCop::update(float dt){
    scene->update(dt);
}

void RobberCop::draw() const {
    scene->draw();
}


// id=0 var=btn1 val=pressed;