#include "drawableObject.h"


DrawableObject::DrawableObject(const std::string& t, float w, float h):
texture(t), width(w), height(h) { }


// Inits the drawing of a player
void DrawableObject::init() {    
    // Load the texture to the texturehandle
    sgct::TextureManager::instance()->loadTexure(
        textureHandle, "Tex", "./textures/" + texture, true);
}

// Draws a player with a set position and MVP matrix
void DrawableObject::draw(/*mat4::MVP,*/ float x, float y) const {
    // Set the active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(textureHandle));

    float s = getSize()/2;

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
        glVertex3f(x+width, y-height, 0.0f);

        glTexCoord2d(1, 1);
        glVertex3f(x+width, y+height, 0.0f);

        glTexCoord2d(0, 1);
        glVertex3f(x-width, y+height, 0.0f);

        glTexCoord2d(0, 0);
        glVertex3f(x-width, y-height, 0.0f);

    glEnd();

}

// Sets the size of a player
void DrawableObject::setSize(float s) {
    size = s;
}

// Gets the size of a player
float DrawableObject::getSize() const {
    return size;
}
