#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"


class DrawableObject {
public:

    DrawableObject() {};
    //DrawableObject(std::string t);

    void draw(/*glm::mat4 MVP,*/ glm::vec2 position = glm::vec2(0.0f, 0.0f)) const;
    void init();
    void setSize(float s);
    float getSize() const;

    //Have this public in order for OpenGL to change it
    size_t textureHandle;

private:
    
    float size;
    std::string texture;
};

#endif // DRAWABLEOBJECT_H