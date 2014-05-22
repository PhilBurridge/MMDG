#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>
#include <map>

#include "debug.h"
#include "sgct.h"
#include "robberCop.h"
#include "player.h"
#include "DrawableSquare.h"
#include "ModelMesh.h"

class Player;
class Scene {


public:
    Scene();
    void update(float dt);
    void checkCollisions();
    void addPlayer(int id, Player *);
    bool removePlayer(int id);
    Player * getPlayer(int id);
    unsigned int getNumberOfPlayers();

    void draw(bool drawSpherical = false);
    
private:
    DrawableSquare *background;
    ModelMesh *bg_sphere;

    int width;
    int height;

    std::map<int, Player *> players;
};

#endif // SCENE_H