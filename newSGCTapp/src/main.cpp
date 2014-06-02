#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"
#include "robberCop.h"

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


/*** Shared variables ***/
sgct::SharedDouble curr_time(0.0);
sgct::SharedBool _drawSpherical(false);

sgct::SharedVector<SharedPlayer> sharedUserData;
std::vector<SharedPlayer> sharedUserDataCopy(MAX_USERS);


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
    }
}

// Write all shared variables to be shared across the cluster
void encode() {    
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &_drawSpherical );
    sgct::SharedData::instance()->writeVector( &sharedUserData );
}

// Read all shared variables to be shared across the cluster
void decode() {    
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &_drawSpherical );
    sgct::SharedData::instance()->readVector( &sharedUserData );
}

void postSyncPreDraw(){
    if (!gEngine->isMaster()){
        sharedUserDataCopy = sharedUserData.getVal();
    }
    for (std::vector<SharedPlayer>::iterator i = sharedUserDataCopy.begin(); i != sharedUserDataCopy.end(); ++i){
        std::cout << "phi = " << i->phi << " theta = " << i->theta << " cop = " << i->cop << std::endl;
    }
}

// Draw function
int degrees = 0;
float size = 0;
void draw() {
    glPushMatrix();
        float s = pow(2.0f, size);
        glScalef(s,s,s);
        glRotatef(degrees, 0, 0, 1);
        robberCop->draw(_drawSpherical.getVal());
    glPopMatrix();
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