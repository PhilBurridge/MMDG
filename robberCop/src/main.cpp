
#include "sgct.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "core.h"
#include "Scene.h"

// Create pointer to the sgct engine
sgct::Engine * gEngine;
Scene *scene;
Core *core;

void init();
void draw();
void preSync();
void encode();
void decode();
void cleanUp();
void externalControlCallback(
    const char * recievedChars, 
    int size, 
    int clientId
    );

/*** Shared variables ***/
// The current time on master
sgct::SharedDouble curr_time(0.0);

int main( int argc, char* argv[] ) {
    // Allocate
    gEngine = new sgct::Engine( argc, argv );

    // Bind functions
    gEngine->setInitOGLFunction(init);
    gEngine->setDrawFunction(draw);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setCleanUpFunction(cleanUp);
    //gEngine->setPostSyncPreDrawFunction(postSyncPreDraw);
    gEngine->setExternalControlCallback(externalControlCallback);

    // Set the functions that handles the shared variables accros the cluster
    sgct::SharedData::instance()->setEncodeFunction(encode);
    sgct::SharedData::instance()->setDecodeFunction(decode);

    // Init the engine
    if(!gEngine->init()) {
        delete gEngine;
        return EXIT_FAILURE;
    }

    // Send a message to the connected clients
    gEngine->sendMessageToExternalControl(core->getMessage());

    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete gEngine;

    // Exit program
    exit(EXIT_SUCCESS);
}

void init() {
    scene->init();
}

void draw() {
    scene->draw();
}

void preSync() {
    // Set the time only on the master
    if( gEngine->isMaster() ) {
        // Get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());
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
    // Deletes the gEngine object
    delete gEngine;
}

void externalControlCallback(const char * recievedChars, int size, int clientId)
    // Only do something with the received message if the game is master
    if(gEngine->isMaster()) {
        // Calls the message handler function from the core class which decodes the received messages
        core->interpret(recievedChars, size, clientId);
    }
}
