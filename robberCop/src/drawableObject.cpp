#include "sgct.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


// Draws a player with a set position and MVP matrix
void DrawableObject::draw(/*mat4::MVP,*/ glm::vec2 position) {
    
    // Set the active texture unit
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(textureHandle));

    // Draw the player polygon
    glBegin(GL_QUADS);
        // Set the normal of the polygon
        glNormal3f(0.0, 0.0, 1.0);
        // Set starting position of the texture mapping
        // The polygon is drawn from the world coordinates perspective 
        // (we set the origin in the center of the polygon)
        // while the texture is drawn from the polygons coordinates 
        // (we draw from the bottom-left corner of the polygon)
        glTexCoord2d(0, 0);
        glVertex3f(position.x - (size / 2), position.y - (size / 2), 0.0f);
        glTexCoord2d(0, 1);
        glVertex3f(position.x - (size / 2), position.y + (size / 2), 0.0f);
        glTexCoord2d(1, 1);
        glVertex3f(position.x + (size / 2), position.y + (size / 2), 0.0f);
        glTexCoord2d(1, 0);
        glVertex3f(position.x + (size / 2), position.y - (size / 2), 0.0f);
    glEnd();
}

// Inits the drawing of a player
void DrawableObject::init() {

    // Set the filter size of texture (just makes it look better)
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    // Set the compression to be used on the texture
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);

    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        textureHandle, "Tex", "./textures/box.png", true);

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

// Sets the size of a player
void DrawableObject::setSize(float s) {
    size = s;
}

// Gets the size of a player
float DrawableObject::getSize() {
    return size;
}
