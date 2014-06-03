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

// Draw function
void draw() {
    robberCop->draw(_drawSpherical.getVal());
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

        // Update shared player positions
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

// Write all shared variables to be shared across the cluster
// These are shared from the master
void encode() {    
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &_drawSpherical );
    sgct::SharedData::instance()->writeVector( &_playersPositions );
    sgct::SharedData::instance()->writeString( &_externalInput );
}

// Read all shared variables to be shared across the cluster
// These are read by the nodes
void decode() {    
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &_drawSpherical );
    sgct::SharedData::instance()->readVector( &_playersPositions );
    sgct::SharedData::instance()->readString( &_externalInput );
}

// Deletes all objects when the program is shut down
void cleanUp() {
    //delete robberCop;
    //delete gEngine;
}

// Receives messages from the TPC connection
void externalControlCallback(const char * recievedChars, int size, int clientId) {
    // Only do something with the received message if master
    if(gEngine->isMaster()) {
        externalInputTemp = std::string(recievedChars);
        // Calls the message handler function from the core class which decodes the received messages
        robberCop->handleExternalInput(recievedChars, size, clientId);
    }
}

// Handles input from keyboard
void keyCallBack(int key, int action){
    if(gEngine->isMaster() && action == GLFW_PRESS) {
        switch(key) {
            case 'S':
                _drawSpherical.toggle();
            break;
            case 'T':
                robberCop->printPingStats();
            break;
            case 'P':
                std::cout << "Pinging clients " << std::endl;
                robberCop->startBenchmark();
            break;
        }
    }
}