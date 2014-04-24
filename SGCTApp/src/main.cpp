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
void startBenchmark();
void stopBenchmark();
void getPingStats(double &min, double &max, double &avg);



/*** Global variables ***/
// Something to hold the texture
size_t textureHandleQRBox;
// Create a pointer to the box displaying our QRCode
sgct_utils::SGCTBox *QRBox = NULL;
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


/* BENCHMARKING VARIABLES */
clock_t startClock;
clock_t endClock;
const std::string PING_MESSAGE = "ping\r\n";
std::vector<double> pingResponses;


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

    pingResponses = std::vector<double>();

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
    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        textureHandleQRBox, "QRBox", "./textures/qrcode.png", true);

    // Create the box object (assign a SGCTBox to the box-pointer)
    QRBox = new sgct_utils::SGCTBox(2.0f, sgct_utils::SGCTBox::Regular);

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
    if(showQRCode) {
        /* DO BOX STUFF */

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
    } else {
        /* TRIANGLE:
        Swaps the direcion and color of the triangle
        when the user presses buttons.
        Now it only swaps the sign of the angle,
        for example 45 -> -45 degrees
        Doesn't look great, but works in this case
        */
        if(clockWise.getVal() == false) {
            speed *= -1;
            glColor3f(1.0f, 0.0f, 0.0f);
        } else {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        // Rotation of the triangle
        glTranslatef(x_coord.getVal(), y_coord.getVal(), -3.0f);
        glRotatef(static_cast<float>(curr_time.getVal()) * speed, 0.0f, 1.0f, 0.0f);
        // Draw the triangle
        glBegin(GL_TRIANGLES);
            glVertex3f(-0.5, -0.5, 0.0f);
            glVertex3f(0.0, 0.5, 0.0f);
            glVertex3f(0.5, -0.5, 0.0f);
        glEnd();
    }
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
    if(QRBox != NULL)
        delete QRBox;
}

/**
This function decodes messages sent to this app by another client

To prevent NULL pointer errors the length of the received message will be checked as well
(unnecessary in this case but a good practice if more complex messages will be added).
- From sgct wiki.
*/
void externalControlCallback(
    const char * recievedChars, int size, int clientId) {
    // How fast the controlled objects move
    float moveSpeed = 1.0f;
    // Only decode the messages if this is the master
    // getDt() is an SGCT-function build in to the engine that gets the delta time
    if(gEngine->isMaster()) {
        // Check length of message and print it
        if(size != 0) {
            std::cout << std::endl << "message: " << recievedChars << std::endl << std::endl;
        }
        // Check the length of the messages and what they contain
        // Set clockWise true or false, value 1 = true and value 0 = false
        if(size == 7 && strncmp(recievedChars, "value", 5) == 0) {
            clockWise.setVal(strncmp(recievedChars + 6, "1", 1) == 0);
        }
        // Sets the "move" variables which translates a players avatar, up down, left or right
        if(size == 2 && strncmp(recievedChars, "up", 2) == 0) {
            y_coord.setVal(y_coord.getVal() + moveSpeed * gEngine->getDt());
        }
        if(size == 4 && strncmp(recievedChars, "down", 4) == 0) {
            y_coord.setVal(y_coord.getVal() - moveSpeed * gEngine->getDt());
        }
        if(size == 4 && strncmp(recievedChars, "left", 4) == 0) {
            x_coord.setVal(x_coord.getVal() - moveSpeed * gEngine->getDt());
        }
        if(size == 5 && strncmp(recievedChars, "right", 5) == 0) {
            x_coord.setVal(x_coord.getVal() + moveSpeed * gEngine->getDt());
        }
        if(size == 7 && strncmp(recievedChars, "value", 5) == 0){
            clockWise.setVal(strncmp(recievedChars + 6, "1", 1) == 0);
        }
        if(size == 8 && strncmp(recievedChars, "pingback", 8) == 0){
            stopBenchmark();
        }
    }
}

/**
This function handles keyboard presses
*/
void keyCallBack(int key, int action) {
    // Check if SGCT is master
    if(gEngine->isMaster() && action == GLFW_PRESS) {
        switch(key) {

            case 'T':
                double min, max, avg;
                getPingStats(min,max,avg);
                std::cout << "Number of ping responses: " << pingResponses.size() << std::endl;
                std::cout << "min: " << min << " ms" << std::endl;
                std::cout << "max: " << max << " ms" << std::endl;
                std::cout << "avg: " << avg << " ms" << std::endl;
            break;

            case 'P':
                std::cout << "Pinging clients " << std::endl;
                startBenchmark();
            break;

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

/**
* Will start the benchmark timer when called
*/
void startBenchmark() {
    pingResponses.clear();
    gEngine->sendMessageToExternalControl( PING_MESSAGE );
    startClock = clock();
}

/**
* Will stop and store the benchmark time when called
*/
void stopBenchmark() {
    endClock = clock();
    double secondsElapsedClock = (endClock - startClock) / (double)(CLOCKS_PER_SEC);
    double millisElapsedClock = 1000*secondsElapsedClock;
    pingResponses.push_back(millisElapsedClock);
}

/**
*  Gets stats from stored benchmarks times
*/
void getPingStats(double &min, double &max, double &avg){
    if(!pingResponses.size()){
        std::cout << "No ping data collected. Press P to ping clients" << std::endl;
        return;
    }

    double tmp = pingResponses[0];
    double sum = tmp;
    min = tmp;
    max = tmp;

    for (int i = 1; i < pingResponses.size(); ++i){
        tmp = pingResponses[i];
        sum += tmp;
        if(tmp < min) {
            min = tmp;
        }
        else if(tmp > max){
            max = tmp;
        }
    }

    avg = sum / pingResponses.size();
}

