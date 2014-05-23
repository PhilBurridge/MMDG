#include "drawableObject.h"

DrawableObject::DrawableObject(const std::string& t, float w, float h):
textureName(t), width(w), height(h) {
    std::cout << "DrawableObject constructor" << std::endl;
    size = fmax(w,h);
}

DrawableObject::DrawableObject(const std::string& t, float s):
textureName(t), size(s) {
    std::cout << "DrawableObject constructor" << std::endl;
    width = s;
    height = s;
}


// Draws a player with a set position and MVP matrix
void DrawableObject::draw(float x, float y, float z) const {
    // Set the active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));

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
        glVertex3f(x+width, y-height, z);

        glTexCoord2d(1, 1);
        glVertex3f(x+width, y+height, z);

        glTexCoord2d(0, 1);
        glVertex3f(x-width, y+height, z);

        glTexCoord2d(0, 0);
        glVertex3f(x-width, y-height, z);

    glEnd();
}

void DrawableObject::drawSphereical(float r, float theta, float phi) const{
    //TO IMPLEMENT
}

// Sets the size of a player
void DrawableObject::setSize(float s) {
    size = s;
}

// Gets the size of a player
float DrawableObject::getSize() const {
    return size;
}

void DrawableObject::display() const{
    std::cout << "size = " << size << std::endl;
    std::cout << "width = " << width << std::endl;
    std::cout << "height = " << height << std::endl;
    std::cout << "textureName = " << textureName << std::endl;
}
