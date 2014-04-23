/*******************************************************
For this example-app legacy openGL is used, just to make
things more easy to follow.
It also dodges some issues in 3.3 and beyond
(Due to lack of skills in the modern versions
and primary to reduce lines of code)

To toggle the QRCode: Use button 'Q' to turn it off and
                      button 'W' to turn it on

The QRCode currnetly takes you to the following website:
http://whendidegthrowlast.com/
Funny if you like DOTA
*******************************************************/

#include "sgct.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "core.h"
//#include "sgct/SGCTNetwork.h"

// Create pointer to the sgct engine
sgct::Engine * gEngine;

void init();
void draw();
void preSync();
void encode();
void decode();
void cleanUp();
void externalControlCallback(const char * recievedChars,int size,int clientId);
void keyCallBack(int key, int action);

/*** Global variables ***/
// Something to hold the texture
size_t textureHandleQRBox;
// Create a pointer to the box displaying our QRCode
//sgct_utils::SGCTBox *QRBox = NULL;
// Is the QRCode visible or not?
bool showQRCode = true;

/*** Shared Global variables ***/
// The current time on master
sgct::SharedDouble curr_time(0.0);
// Is the object spinning clockwise or not?
sgct::SharedBool clockWise(false);
// x-coord for a players avatar
sgct::SharedFloat x_coord(0.0);
// y-coord for a players avatar
sgct::SharedFloat y_coord(0.0);
// z-coord for a players avatar
sgct::SharedFloat z_coord(0.0);

Core core;

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
    gEngine->setKeyboardCallbackFunction(keyCallBack);

    // Set the functions that handles the shared variables accros the cluster
    sgct::SharedData::instance()->setEncodeFunction(encode);
    sgct::SharedData::instance()->setDecodeFunction(decode);

    // Init the engine
    if(!gEngine->init()) {
        delete gEngine;
        return EXIT_FAILURE;
    }

    std::string testString = "id=1 var=btn1 val=pressed;id=2 var=slider1 val=123;";
    core.handleExternalInput(testString.c_str(), testString.length(), -1);

    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete gEngine;

    // Exit program
    exit(EXIT_SUCCESS);
}

void init() {
    // Set the filter size of texture (just makes it look better)
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    // Set the compression to be used on the texture
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);

    //sgct::TextureManager::instance()->setWarpingMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        textureHandleQRBox, "QRBox", "./textures/box.png", true);

    // Create the box object (assign a SGCTBox to the box-pointer)
    //QRBox = new sgct_utils::SGCTBox(2.0f, sgct_utils::SGCTBox::Regular);

    // Enable some openGL stuff
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_COLOR_MATERIAL );
    glDisable( GL_LIGHTING );
    glEnable( GL_TEXTURE_2D );

    // Set up backface culling
    glCullFace(GL_BACK);
    // The polygon winding is counter clockwise (CCW)
    glFrontFace(GL_CCW);
}

void draw() {
    // Set the spinning speed
    float speed = 25.0f;

    // Shall the QRCode be drawn or not?
    /*if(showQRCode) {
        // DO BOX STUFF 

        // Translate the box
        glTranslatef(0.0f, 0.0f, -4.0f);
        // Set the base color of the QRBox
        // (white, but will not matter in this case)
        glColor3f(1.0f, 1.0f, 1.0f);

        // Set and bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(textureHandleQRBox));

        // Draw the QRBox
        QRBox->draw();
        */

        /* TRIANGLE:
        Swaps the direcion and color of the triangle
        when the user presses buttons.
        Now it only swaps the sign of the angle,
        for example 45 -> -45 degrees
        Doesn't look great, but works in this case
        */
        if(clockWise.getVal() == false) {
            speed *= -1;
            //glColor3f(1.0f, 0.0f, 0.0f);
        } else {
            //glColor3f(0.0f, 1.0f, 0.0f);
        }
        // Rotation of the triangle
        glTranslatef(x_coord.getVal(), y_coord.getVal(), -3.0f);
        //glRotatef(static_cast<float>(curr_time.getVal()) * speed, 0.0f, 1.0f, 0.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(textureHandleQRBox));

        // Draw the triangle
        glBegin(GL_QUADS);
            glNormal3f(0.0, 0.0, 1.0);
            glTexCoord2d(0, 0);
            glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2d(0, 1);
            glVertex3f(-1.0f, 1.0f, 0.0f);
            glTexCoord2d(1, 1);
            glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2d(1, 0);
            glVertex3f(1.0f, -1.0f, 0.0f);
        glEnd();
    
}

// Set the time only on the master
void preSync() {
    if( gEngine->isMaster() ) {
        // get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());
    }
}

// Write all shared variables to be shared across the cluster
void encode() {
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &clockWise );
}

// Read all shared variables to be shared across the cluster
void decode() {
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &clockWise );
}

// Remove QRBox object when exiting the program
void cleanUp() {
    /*if(QRBox != NULL)
        delete QRBox;
        */
}

/**
This function decodes messages sent to this app by another client

To prevent NULL pointer errors the length of the received message will be checked as well
(unnecessary in this case but a good practice if more complex messages will be added).
- From sgct wiki.
*/
void externalControlCallback(const char * recievedChars, int size, int clientId) {

    core.handleExternalInput(recievedChars, size, clientId);

    
}

/**
This function handles keyboard presses
*/
void keyCallBack(int key, int action) {
    // Check if SGCT is master
    if(gEngine->isMaster()) {
        switch(key) {
            case 'Q':
                std::cout << "Q PRESSED - QR TURNED OFF" << std::endl;
                // Toggle the QRCode on button press (Make it disappear)
                if(showQRCode) {
                    showQRCode = false;
                }
            break;

            case 'E':
                std::cout << "W PRESSED - QR TURNED ON" << std::endl;
                // Toggle the QRCode on button press (Make it appear)
                if(!showQRCode) {
                    showQRCode = true;
                }
            break;
        }
    }
}
