#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
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
    // Constructor
    RobberCop(sgct::Engine * gEngine); 

    // Initiates robberCop
    void init();

    // Handles all actions of robberCop
    virtual void process(int id, std::string action, std::string value);
    
    void update(float dt);
    void draw(bool drawSpherical) const;

    void toggleDrawSpherical();
    
    Scene *scene;

private:
	bool drawSpherical;
};

#endif // ROBBERCOP_H