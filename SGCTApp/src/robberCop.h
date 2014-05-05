#include <stdlib.h>
#include <stdio.h>
#include "sgct.h"
#include "scene.h"
#include "debug.h"
//#include "player.h"
#include "core.h"

#ifndef ROBBERCOP_H
#define ROBBERCOP_H

class Scene;

class RobberCop: public Core{
private:
    

public:
    RobberCop();
    RobberCop(sgct::Engine * gEngine); 
    virtual void process(int id, std::string action, std::string value);
    Scene *scene;
    void draw(size_t texturehandle);
};

#endif // ROBBERCOP_H