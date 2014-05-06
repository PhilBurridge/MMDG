#include "robberCop.h"

size_t RobberCop::box_texture_id = -1;
size_t RobberCop::rob_texture_id = -1;
size_t RobberCop::cop_texture_id = -1;

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

    // TODO: Ladda först in texturer, låt sedan drawableObject referera till texture_id

    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        RobberCop::box_texture_id, "QRBox", "./textures/box.png", true);
    sgct::TextureManager::instance()->loadTexure(
        RobberCop::cop_texture_id, "cop", "./textures/box.png", true);
    sgct::TextureManager::instance()->loadTexure(
        RobberCop::rob_texture_id, "cop", "./textures/box.png", true);


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
        debug
        Player *p = new Player(glm::vec2(1.0f, 0.0f), false);
        debug
        //sgct::TextureManager::instance()->loadTexure(
        //    p->textureHandle, "player0", "./textures/" + p->texture, true);

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