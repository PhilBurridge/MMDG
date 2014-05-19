#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"
#include "robberCop.h"



/* !!!! FIXA ROBERCOP I CORE BRANCHEN !!!! */


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


int main( int argc, char* argv[] ) {
    // Allocate
    gEngine = new sgct::Engine( argc, argv );
    robberCop = new RobberCop(gEngine);

    // Bind functions
    gEngine->setInitOGLFunction(init);
    gEngine->setDrawFunction(draw);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setCleanUpFunction(cleanUp);
    //gEngine->setPostSyncPreDrawFunction(postSyncPreDraw);
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

    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete robberCop;
    //delete scene;
    delete gEngine;

    // Exit program
    exit(EXIT_SUCCESS);
}

void init() {
    std::cout << "  ** MAIN INIT **  " << std::endl;
    robberCop->init();
    std::cout << "  ** MAIN INIT DONE **  " << std::endl;
}

void draw() {
    robberCop->draw();
}

void preSync() {
    // Set the time only on the master
    if( gEngine->isMaster() ) {
        // Get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());

        robberCop->update(gEngine->getDt());
    }
}

// Write all shared variables to be shared across the cluster
void encode() {    
    sgct::SharedData::instance()->writeDouble( &curr_time );
}

// Read all shared variables to be shared across the cluster
void decode() {    
    sgct::SharedData::instance()->readDouble( &curr_time );
}

// Deletes all objects when the program is shut down
void cleanUp() {
    // add stuff
}

void externalControlCallback(const char * recievedChars, int size, int clientId) {
    // Only do something with the received message if the game is master
    if(gEngine->isMaster()) {
        // Calls the message handler function from the core class which decodes the received messages
        robberCop->handleExternalInput(recievedChars, size, clientId);
    }
}

void keyCallBack(int key, int action){
    if(gEngine->isMaster() && action == GLFW_PRESS) {
        switch(key) {
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