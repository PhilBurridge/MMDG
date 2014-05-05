#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"


class DrawableObject {
public:

    DrawableObject(const std::string& texture, float w = 1.0f, float h = 1.0f);
    //DrawableObject(std::string t);

    void draw(/*glm::mat4 MVP,*/ float x=0.0f, float y=0.0f) const;
    void init();
    void setSize(float s);
    float getSize() const;

    //Have this public in order for OpenGL to change it
    size_t textureHandle;

private:
    
    float size;
    float width;
    float height;
    const std::string texture;
};

#endif // DRAWABLEOBJECT_H