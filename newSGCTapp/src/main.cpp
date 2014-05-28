#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"
#include "robberCop.h"


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

/*** Shared variables ***/
// The current time on master
sgct::SharedDouble curr_time(0.0);
sgct::SharedBool _drawSpherical(false);
sgct::SharedVector<glm::vec2> _playersPositions;
sgct::SharedString _externalInput;
std::string externalInputTemp = "";


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

    // Send a message to the connected clients
    //gEngine->sendMessageToExternalControl(core->getMessage());

    //test
    //robberCop->process(1,"connected","1");
    //int x_res = gEngine->getActiveXResolution();
    //std::cout << "X-RES: " << x_res << std::endl;
    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete robberCop;
    //delete scene;
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

// Sets the time and timeintervall(dt)
void preSync() {
    // Set the time only on the master
    if( gEngine->isMaster() ) {
        // Get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());

        //Update shared external input
        _externalInput.setVal(externalInputTemp);
        externalInputTemp = "";

        //update shared player positions
        
        Player * p;
        std::vector<glm::vec2> playerPositionsTemp;
        const std::map<int, Player*> playerMap = robberCop->scene->getPlayerMap();
        for(std::map<int, Player *>::const_iterator it = playerMap.begin(); it != playerMap.end(); it++) {
            p = it->second;
            playerPositionsTemp.push_back(p->getPosition());
        }
        _playersPositions.setVal(playerPositionsTemp);
    }
    else {
        
        std::string extStr = _externalInput.getVal();
        if(extStr != "")
            robberCop->handleExternalInput(extStr.c_str(), extStr.length(), 0);

        robberCop->scene->setPlayerPositions(_playersPositions.getVal());
    }

    robberCop->update(gEngine->getDt());
}

void postSyncPreDraw(){
    
}

// Write all shared variables to be shared across the cluster
void encode() {    
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &_drawSpherical );
    sgct::SharedData::instance()->writeVector( &_playersPositions );
    sgct::SharedData::instance()->writeString( &_externalInput );
}

// Read all shared variables to be shared across the cluster
void decode() {    
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &_drawSpherical );
    sgct::SharedData::instance()->readVector( &_playersPositions );
    sgct::SharedData::instance()->readString( &_externalInput );
}

// Deletes all objects when the program is shut down
void cleanUp() {
    // add stuff
}

// Receives messages from the TPC connection
void externalControlCallback(const char * recievedChars, int size, int clientId) {
    // Only do something with the received message if the game is master
    if(gEngine->isMaster()) {
        // Calls the message handler function from the core class which decodes the received messages

        externalInputTemp = std::string(recievedChars);
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