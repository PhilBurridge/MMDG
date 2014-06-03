#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"
#include "robberCop.h"
#include "player.h"

#define MAX_USERS 256

// Create pointer to the sgct engine, scene and core
sgct::Engine * gEngine;
RobberCop *robberCop;

void init();
void draw();
void preSync();
void postSyncPreDraw();
void encode();
void decode();

void cleanUp();
void externalControlCallback(const char * recievedChars, int size, int clientId);
void keyCallBack(int key, int action);

struct SharedPlayer{
    float phi;
    float theta;
    bool cop;
};

void drawPlayers(const std::vector<SharedPlayer> &v, bool drawSpherical);
void drawPlayer(const SharedPlayer &sp);
void drawPlayerSpherical(const SharedPlayer &sp);

float Player::radius = 2.0f;
float width = 0.15f;
float height = 0.15f;

std::vector<SharedPlayer> sharedUserDataCopy(MAX_USERS);

/*** Shared variables ***/
sgct::SharedDouble curr_time(0.0);
sgct::SharedBool _drawSpherical(false);
sgct::SharedInt nPlayers(0);
sgct::SharedVector<SharedPlayer> sharedUserData;



int main( int argc, char* argv[] ) {
    // Allocate
    gEngine = new sgct::Engine( argc, argv );
    robberCop = new RobberCop(gEngine);

    // Bind functions
    gEngine->setInitOGLFunction(init);
    gEngine->setDrawFunction(draw);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setCleanUpFunction(cleanUp);
    gEngine->setPostSyncPreDrawFunction(postSyncPreDraw);
    gEngine->setExternalControlCallback(externalControlCallback);
    gEngine->setKeyboardCallbackFunction(keyCallBack);

    // Set the functions that handles the shared variables accros the cluster
    sgct::SharedData::instance()->setEncodeFunction(encode);
    sgct::SharedData::instance()->setDecodeFunction(decode);

    // Init the engine
    if(!gEngine->init()) {
        delete gEngine;
        return EXIT_FAILURE;
    }

    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete robberCop;
    delete gEngine;

    // Exit program
    exit(EXIT_SUCCESS);
}

// Initiation function
void init() {
    std::cout << "  ** MAIN INIT **  " << std::endl;
    robberCop->init();
    std::cout << "  ** MAIN INIT DONE **  " << std::endl;
}

void preSync() {
    sharedUserDataCopy.clear();
    // Set the time only on the master
    if( gEngine->isMaster() ) {
        // Get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());

        robberCop->update(gEngine->getDt());

        //update shared player positions
        Player * p;
        const std::map<int, Player*> playerMap = robberCop->scene->getPlayerMap();
        for(std::map<int, Player *>::const_iterator it = playerMap.begin(); it != playerMap.end(); it++) {
            p = it->second;
            
            //Create an instance of the shared player data
            SharedPlayer sp;
            sp.phi = p->getPosition().x;
            sp.theta = p->getPosition().y;
            sp.cop = p->isCop();

            sharedUserDataCopy.push_back(sp);
        }
        sharedUserData.setVal(sharedUserDataCopy);

        nPlayers.setVal(playerMap.size());
    }
}

// Write all shared variables to be shared across the cluster
void encode() {    
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &_drawSpherical );
    sgct::SharedData::instance()->writeInt( &nPlayers );
    sgct::SharedData::instance()->writeVector( &sharedUserData );
}

// Read all shared variables to be shared across the cluster
void decode() {    
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &_drawSpherical );
    sgct::SharedData::instance()->readInt( &nPlayers );
    sgct::SharedData::instance()->readVector( &sharedUserData );
}

void postSyncPreDraw(){
    if (!gEngine->isMaster()){
        sharedUserDataCopy = sharedUserData.getVal();
    }
}

// Draw function
int degrees = 0;
float size = 0;
void draw() {
    glEnable( GL_TEXTURE_2D );
    glActiveTexture(GL_TEXTURE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        float s = pow(2.0f, size);
        glScalef(s,s,s);
        glRotatef(degrees, 0, 0, 1);
        robberCop->draw(_drawSpherical.getVal());
        drawPlayers(sharedUserDataCopy, _drawSpherical.getVal());
    glPopMatrix();

    

    glDisable( GL_TEXTURE_2D );
}


// Deletes all objects when the program is shut down
void cleanUp() {
    
}

// Receives messages from the TPC connection
void externalControlCallback(const char * recievedChars, int size, int clientId) {
    // Only do something with the received message if the game is master
    if(gEngine->isMaster()) {
        // Calls the message handler function from the core class which decodes the received messages
        robberCop->handleExternalInput(recievedChars, size, clientId);
    }
}

// Handles input from keyboard
void keyCallBack(int key, int action){
    if(gEngine->isMaster() && action == GLFW_PRESS) {
        switch(key) {
            case 'A': degrees -= 10; break;
            case 'D': degrees += 10; break;
            case 'Z': size--; break;
            case 'X': size++; break;

            case 'S':
                _drawSpherical.toggle(); break;
            case 'T':
                robberCop->printPingStats(); break;


            case 'P':
                std::cout << "Pinging clients " << std::endl;
                robberCop->startBenchmark();
            break;
        }
    }
}

void drawPlayers(const std::vector<SharedPlayer> &v, bool drawSpherical){
    if (drawSpherical){
        for (std::vector<SharedPlayer>::iterator i = sharedUserDataCopy.begin(); i != sharedUserDataCopy.end(); ++i){
            drawPlayerSpherical(*i);
        }
    }
    else{
        for (std::vector<SharedPlayer>::iterator i = sharedUserDataCopy.begin(); i != sharedUserDataCopy.end(); ++i){
            drawPlayer(*i);
        }
    }
}

void drawPlayer(const SharedPlayer &sp){
    float x = sp.phi;
    float y = sp.theta;
    float z = Player::radius;



    glActiveTexture(GL_TEXTURE0);


    // Bind the texture by its set handle
    std::string textureName = sp.cop ? "cop" : "rob";
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));

    glPushMatrix();
    glTranslatef(x,y,z);

    // Draw the player polygon
    glBegin(GL_QUADS);
        // Set the normal of the polygon
        glNormal3f(0.0, 0.0, 1.0);

        // Set starting position of the texture mapping
        // The polygon is drawn from the world coordinates perspective 
        // (we set the origin in the center of the polygon)
        // while the texture is drawn from the polygons coordinates 
        // (we draw from the bottom-left corner of the polygon)

        // Define polygon vertices in counter clock wise order
        glTexCoord2d(1, 0);
        glVertex3f(+width, -height, 0);

        glTexCoord2d(1, 1);
        glVertex3f(+width, +height, 0);

        glTexCoord2d(0, 1);
        glVertex3f(-width, +height, 0);

        glTexCoord2d(0, 0);
        glVertex3f(-width, -height, 0);

    glEnd();

    glPopMatrix();

}

void drawPlayerSpherical(const SharedPlayer &sp){
    float phi = -sp.phi;
    float theta = sp.theta;
    float r = -Player::radius;

    float x = r*glm::sin(phi)*glm::cos(theta);
    float y = r*glm::sin(phi)*glm::sin(theta);
    float z = r*glm::cos(phi);

    /*std::cout << "r    =" << r << std::endl;
    std::cout << "phi  =" << phi << std::endl;
    std::cout << "theta=" << theta << std::endl;
    std::cout << "x=" << x << std::endl;
    std::cout << "y=" << y << std::endl;
    std::cout << "z=" << z << std::endl;
    std::cout << "----" << std::endl;*/


    glActiveTexture(GL_TEXTURE0);
    std::string textureName = sp.cop ? "cop" : "rob";
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));

    glPushMatrix();
        glRotatef(180.0f*phi/3.1415f, 0,1,0);
        glRotatef(180.0f*theta/3.1415f, 1,0,0);
        glTranslatef(0,0,r);
        
        // Draw the player polygon
        glBegin(GL_QUADS);
            // Set the normal of the polygon
            glNormal3f(0.0, 0.0, 1.0);

            // Set starting position of the texture mapping
            // The polygon is drawn from the world coordinates perspective 
            // (we set the origin in the center of the polygon)
            // while the texture is drawn from the polygons coordinates 
            // (we draw from the bottom-left corner of the polygon)

            // Define polygon vertices in counter clock wise order
            glTexCoord2d(1, 0);
            glVertex3f(+width, -height, 0);

            glTexCoord2d(1, 1);
            glVertex3f(+width, +height, 0);

            glTexCoord2d(0, 1);
            glVertex3f(-width, +height, 0);

            glTexCoord2d(0, 0);
            glVertex3f(-width, -height, 0);
        glEnd();
    glPopMatrix();
}


