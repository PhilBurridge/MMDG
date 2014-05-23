#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"


class DrawableObject {
public:

    // Constructor for objects
    DrawableObject(const std::string& texture, float w, float h);
    DrawableObject(const std::string& texture, float s);

    // Handles the drawing of an object
    void draw(/*glm::mat4 MVP,*/ float x=0.0f, float y=0.0f, float z=0.0f) const;
    void drawSphereical(float r = 1.0f, float theta = 0.0f, float phi = 0.0f) const;

    // Sets, returns and displays the properties of a player
    void setSize(float s);
    float getSize() const;
    void display() const;

    //Have this public in order for OpenGL to change it
    std::string textureName;

private:
    // Object variables
    float size;
    float width;
    float height;
};

#endif // DRAWABLEOBJECT_H