#ifndef DrawableSquare_H
#define DrawableSquare_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"


class DrawableSquare {
public:


    DrawableSquare(const std::string& texture, float w, float h);
    DrawableSquare(const std::string& texture, float s);

    void draw(/*glm::mat4 MVP,*/ float x=0.0f, float y=0.0f, float z=0.0f) const;
    void drawSphereical(float r = 1.0f, float theta = 0.0f, float phi = 0.0f) const;
    void setSize(float s);
    float getSize() const;
    void display() const;

    //Have this public in order for OpenGL to change it
    std::string textureName;

private:

    float size;
    float width;
    float height;
};

#endif // DrawableSquare_H