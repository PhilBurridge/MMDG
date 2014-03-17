#include "sgct.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "sgct/SGCTNetwork.h"

// Create pointer to the sgct engine
sgct::Engine * gEngine;

void init();
void draw();
void preSync();
void encode();
void decode();
void cleanUp();
void toggleDirection(sgct::SharedBool clockWise);
void externalControlCallback(const char * recievedChars, int size, int clientId);

// Shared vars
sgct::SharedDouble curr_time(0.0);
sgct::SharedBool clockWise(false);

// Global vars
GLuint vertexArray = GL_FALSE;
GLuint vertexPositionBuffer = GL_FALSE;
GLuint vertexColorBuffer = GL_FALSE;

GLint Matrix_Loc = -1;

int main( int argc, char* argv[] ) {
    // Allocate
    gEngine = new sgct::Engine( argc, argv );

    // Bind functions
    gEngine->setInitOGLFunction( init );
    gEngine->setDrawFunction( draw );
    gEngine->setPreSyncFunction( preSync );
    gEngine->setCleanUpFunction( cleanUp );
    gEngine->setExternalControlCallback( externalControlCallback );

    // Set the functions that handles the shared vars accros the cluster
    sgct::SharedData::instance()->setEncodeFunction( encode );
    sgct::SharedData::instance()->setDecodeFunction( decode );

    // Init the engine
    if( !gEngine->init( sgct::Engine::OpenGL_3_3_Core_Profile ) )
    {
        delete gEngine;
        return EXIT_FAILURE;
    }

    // Main loop
    gEngine->render();

    // Clean up (de-allocate)
    delete gEngine;

    // Exit program
    exit( EXIT_SUCCESS );
}

void init() {
    // Set triangle vertices
    const GLfloat vertex_position_data[] = { 
        -0.5f, -0.5f, 0.0f,
         0.0f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    // Set triangle color(s)
    const GLfloat vertex_color_data[] = { 
        1.0f, 0.0f, 0.0f, //red
        0.0f, 1.0f, 0.0f, //green
        0.0f, 0.0f, 1.0f //blue
    };

    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    // Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_position_data), vertex_position_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    //generate VBO for vertex colors
    glGenBuffers(1, &vertexColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
    //upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color_data), vertex_color_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    glBindVertexArray(0); //unbind

    sgct::ShaderManager::instance()->addShaderProgram( "xform",
            "SimpleVertexShader.vertexshader",
            "SimpleFragmentShader.fragmentshader" );

    sgct::ShaderManager::instance()->bindShaderProgram( "xform" );
 
    Matrix_Loc = sgct::ShaderManager::instance()->getShaderProgram( "xform").getUniformLocation( "MVP" );
 
    sgct::ShaderManager::instance()->unBindShaderProgram();
}

void draw() {
    // Speed of the spinning triangle
    float speed = 50.0f;
    
    // Swap the direcion (the ugly way of doing it)
    if(clockWise.getVal() == true) {
        speed = speed;
    } else {
        speed = -speed;
    }

    glm::mat4 sceneMat = glm::rotate( glm::mat4(1.0f), static_cast<float>( curr_time.getVal() ) * speed, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP = gEngine->getActiveModelViewProjectionMatrix() * sceneMat;

    sgct::ShaderManager::instance()->bindShaderProgram( "xform" );
        
    glUniformMatrix4fv(Matrix_Loc, 1, GL_FALSE, &MVP[0][0]);

    glBindVertexArray(vertexArray);
    
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // unbind shaders
    glBindVertexArray(0);
    sgct::ShaderManager::instance()->unBindShaderProgram();
}

void preSync() {
    // this test might not be needed, will leave it for now, set the time only on the master
    //if( gEngine->isMaster() ) {
        // get the time in seconds
        curr_time.setVal(sgct::Engine::getTime());
    //}
}

void encode() {
    sgct::SharedData::instance()->writeDouble( &curr_time );
    sgct::SharedData::instance()->writeBool( &clockWise );
}

void decode() {
    sgct::SharedData::instance()->readDouble( &curr_time );
    sgct::SharedData::instance()->readBool( &clockWise );
}

void cleanUp() {
    if(vertexPositionBuffer)
        glDeleteBuffers(1, &vertexPositionBuffer);
    if(vertexColorBuffer)
        glDeleteBuffers(1, &vertexColorBuffer);
    if(vertexArray)
        glDeleteVertexArrays(1, &vertexArray);
}

/*To prevent NULL pointer errors the length of the received message will be checked as well
 (unnecessary in this case but a good practice if more complex messages will be added).
 - From sgct wiki. 
*/
void externalControlCallback(const char * recievedChars, int size, int clientId) {
    // this test might not be needed, will leave it for now
    //if(gEngine->isMaster()) {
        if(size != 0)
            std::cout << std::endl << "message: " << recievedChars << std::endl << std::endl;
        if(size == 7 && strncmp(recievedChars, "value", 5) == 0)
            clockWise.setVal(strncmp(recievedChars + 6, "1", 1) == 0);
    //}
}
