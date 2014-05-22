#include "DrawableSquare.h"

DrawableSquare::DrawableSquare(const std::string& t, float w, float h):
textureName(t), width(w), height(h) {
    std::cout << "DrawableSquare constructor" << std::endl;
    size = fmax(w,h);
}

DrawableSquare::DrawableSquare(const std::string& t, float s):
textureName(t), size(s) {
    std::cout << "DrawableSquare constructor" << std::endl;
    width = s;
    height = s;
}

// Draws a player with a set position and MVP matrix
void DrawableSquare::draw(/*mat4::MVP,*/ float x, float y, float z) const {
    // Set the active texture unit
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));

    glPushMatrix();
    glTranslatef(x,y,z);

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
        glVertex3f(+width, -height, 0);

        glTexCoord2d(1, 1);
        glVertex3f(+width, +height, 0);

        glTexCoord2d(0, 1);
        glVertex3f(-width, +height, 0);

        glTexCoord2d(0, 0);
        glVertex3f(-width, -height, 0);

    glEnd();

    glPopMatrix();

    drawSphereical(1,x,y);

}

void DrawableSquare::drawSphereical(float r, float theta, float phi) const{

    float theta_player = 2*glm::atan(height, 2*r);
    float phi_player = 2*glm::atan(width, 2*r);

    float x = r*glm::sin(phi)*glm::cos(theta);
    float y = r*glm::sin(phi)*glm::sin(theta);
    float z = r*glm::cos(phi);

    std::cout << "r    =" << r << std::endl;
    std::cout << "phi  =" << phi << std::endl;
    std::cout << "theta=" << theta << std::endl;
    std::cout << "x=" << x << std::endl;
    std::cout << "y=" << y << std::endl;
    std::cout << "z=" << z << std::endl;
    std::cout << "----" << std::endl;


    //std::cout << "theta_player = " << theta_player << std::endl;


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName(textureName));

    glPushMatrix();
    glTranslatef(x,y,z);

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
        glVertex3f(+width, -height, 0);

        glTexCoord2d(1, 1);
        glVertex3f(+width, +height, 0);

        glTexCoord2d(0, 1);
        glVertex3f(-width, +height, 0);

        glTexCoord2d(0, 0);
        glVertex3f(-width, -height, 0);

    glEnd();

    glPopMatrix();
}

// Sets the size of a player
void DrawableSquare::setSize(float s) {
    size = s;
}

// Gets the size of a player
float DrawableSquare::getSize() const {
    return size;
}

void DrawableSquare::display() const{
    std::cout << "size = " << size << std::endl;
    std::cout << "width = " << width << std::endl;
    std::cout << "height = " << height << std::endl;
    std::cout << "textureName = " << textureName << std::endl;
}
