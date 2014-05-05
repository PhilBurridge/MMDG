#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "debug.h"
#include "sgct.h"


class DrawableObject {
public:

    DrawableObject(const std::string& texture, float w, float h);
    DrawableObject(const std::string& texture, float s);
    //DrawableObject(std::string t);

    void draw(/*glm::mat4 MVP,*/ float x=0.0f, float y=0.0f) const;
    void init();
    void setSize(float s);
    float getSize() const;
    void display() const;

    //Have this public in order for OpenGL to change it
    size_t textureHandle;
    const std::string texture;    

private:

    float size;
    float width;
    float height;

    static int objCounter;
    
};

#endif // DRAWABLEOBJECT_H