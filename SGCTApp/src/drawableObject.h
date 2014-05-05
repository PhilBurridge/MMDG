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

    void draw(/*glm::mat4 MVP,*/ glm::vec2 position);
    //void init();
    void setSize(float s);
    float getSize();

private:
    size_t textureHandle;
    float size;
    std::string texture;
};

#endif // DRAWABLEOBJECT_H