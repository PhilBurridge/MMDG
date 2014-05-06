#include "robberCop.h"

RobberCop::RobberCop(sgct::Engine * gEngine):
Core(gEngine) {
    scene = new Scene();
    std::cout << "constructing RobberCop" << std::endl;
};

void RobberCop::init(){

    // --- INIT OPENGL --- // 

    // Set the filter size of texture (just makes it look better)
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    // Set the compression to be used on the texture
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);

    // Load the texture to the texturehandle
    size_t trash = -1;
    sgct::TextureManager::instance()->loadTexure(trash, "mmdg",  "./textures/mmdg.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "box", "./textures/box.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "cop", "./textures/box.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "rob", "./textures/box.png", true);



    // Enable some openGL stuff
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_COLOR_MATERIAL );
    glDisable( GL_LIGHTING );
    glEnable(GL_CULL_FACE);
    glEnable( GL_TEXTURE_2D );

    // Set up backface culling
    glCullFace(GL_BACK);
    // The polygon winding is counter clockwise (CCW)
    glFrontFace(GL_CCW);

    scene->init();
}

// Overiding method from core
void RobberCop::process(int id, std::string var, std::string val) {

    std::cout << "INPUT: " << std::endl;
    Core::process(id,var,val);

    std::cout << "HANDELING INPUT" << std::endl;

    if(var == "connected"){
        std::cout << "adding new player" << std::endl;
        Player *p = new Player(glm::vec2(1.0f, 0.0f), false);
        scene->addPlayer(id, p);
        return;
    }

    if(var == "disconnected"){
        std::cout << "Player disconnected" << std::endl;
        scene->removePlayer(id);
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
        
        
        if(pressed){
            int direction;
            switch(btnNumber){
                case 0: direction = Player::NORTH; break;
                case 1: direction = Player::NORTH_EAST; break;
                case 2: direction = Player::EAST; break;
                case 3: direction = Player::SOUTH_EAST; break;
                case 4: direction = Player::SOUTH; break;
                case 5: direction = Player::SOUTH_WEST; break;
                case 6: direction = Player::WEST; break;
                case 7: direction = Player::NORTH_WEST; break;
                default: std::cout << "ERROR! BAD BUTTON NUMBER" << std::endl;
            }
            scene->getPlayer(id)->setMoveDirection(direction);
        }
        else {
            scene->getPlayer(id)->stop();
        }

    }
}

void RobberCop::update(float dt){
    scene->update(dt);
}

void RobberCop::draw() const {
    scene->draw();
}


// id=0 var=btn1 val=pressed;