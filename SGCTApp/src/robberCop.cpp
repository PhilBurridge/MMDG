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

    // Load the textures to the texturehandle
    size_t trash = -1;
    sgct::TextureManager::instance()->loadTexure(trash, "mmdg",  "./textures/mmdg.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "box", "./textures/box.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "cop", "./textures/cop.png", true);
    sgct::TextureManager::instance()->loadTexure(trash, "rob", "./textures/robber.png", true);



    // Enable some openGL stuff
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_COLOR_MATERIAL );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable( GL_LIGHTING );
    glEnable(GL_CULL_FACE);
    glEnable( GL_TEXTURE_2D );

    // Set up backface culling
    glCullFace(GL_BACK);
    // The polygon winding is counter clockwise (CCW)
    glFrontFace(GL_CCW);
}

// Overiding method from core
void RobberCop::process(int id, std::string var, std::string val) {

    std::cout << "INPUT: " << std::endl;
    Core::process(id,var,val);

    std::cout << "HANDELING INPUT" << std::endl;

    if(var == "connected"){
        std::cout << "adding new player" << std::endl;

        bool isCop;
        // Maybe change the ratio between robbers and cops, now it's 50/50
        // Sets a player to Cop or Robber with a chosen percent
        if((id % 2) == 0) {
            isCop = false;
            std::cout << "Player " << id << " is a robber" << std::endl;
        } else {
            isCop = true;
            std::cout << "Player " << id << " is a cop" << std::endl;
        } 

        // Randomize spawn position, took this from stackoverflow.
        // Might not be the best rng ever.
        //srand (static_cast <unsigned> (time(0)));
        // Randomize spawn position
        float rand_x = ((1.57 - (-1.57)) * ((float) rand() / RAND_MAX)) + (-1.57);
        float rand_y = ((0.78 - (-0.78)) * ((float) rand() / RAND_MAX)) + (-0.78);

        // Create the player
        Player *p = new Player(glm::vec2(rand_x, rand_y), isCop);

        // add the player to the scen
        scene->addPlayer(id, p);
        return;
    }

    // If a Player disconnects remove that Player
    if(var == "disconnected"){
        std::cout << "Player disconnected" << std::endl;
        scene->removePlayer(id);
        return;
    }

    // Get the Players Id
    Player * p = scene->getPlayer(id);

    // Error handler
    if(p == NULL){
        std::cout << "WARNING! TRYING TO ACCESS NULL POINTER" << std::endl;
        return;
    }

    // Checks what button is pressed and then sets the corrensponding direction 
    if(var.substr(0,3) == "btn"){
        int btnNumber = atoi(var.substr(3,1).c_str());
        bool pressed = (val == "1");
        std::cout << "btn number: " << btnNumber << std::endl;
        std::cout << "btn is pressed: " << pressed << std::endl;
            
        // Chooses the corrensponding direction
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
        
        // Check if pressed or released
        if(pressed){
            p->setMoveDirection(direction);
        }
        else { //If button is released

            //Stop player if the button released is the same as the last one pressed
            if (p->getMoveDirection() == direction){
                p->stop();
            }
        }

    }
}

// Updates robberCop
void RobberCop::update(float dt){
    scene->update(dt);
}

// Draws robberCop
void RobberCop::draw() const {
    scene->draw();
}