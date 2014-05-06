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
private:
    

public:
    RobberCop(sgct::Engine * gEngine); 

    virtual void process(int id, std::string action, std::string value);
    
    void update(float dt);
    void draw() const;
    void init();

    Scene *scene;

    static size_t cop_texture_id;
    static size_t rob_texture_id;
    static size_t box_texture_id;
};

#endif // ROBBERCOP_H