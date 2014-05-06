#include <stdlib.h>
#include <stdio.h>
#include "sgct.h"
#include "scene.h"
#include "debug.h"
#include "core.h"

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