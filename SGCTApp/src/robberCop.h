#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "sgct.h"
#include "scene.h"
#include "debug.h"
#include "core.h"
//#include "player.h"

#ifndef ROBBERCOP_H
#define ROBBERCOP_H

class Scene;

class RobberCop: public Core{


public:
    RobberCop(sgct::Engine * gEngine); 

    virtual void process(int id, std::string action, std::string value);
    
    void update(float dt);
    void draw() const;
    void init();
    
    Scene *scene;
};

#endif // ROBBERCOP_H