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
#include "drawableObject.h"

class Player;

class Scene {


public:
    Scene();
    void init();
    void update(float dt);
    void updatePositions(float dt);
    void checkCollisions();
    void addPlayer(int id, Player *);
    Player * getPlayer(int id);

    void draw();
    
private:
    DrawableObject *background;

    int width;
    int height;
    //size_t backgroundhandle;

    std::vector<Player *> player_vec;
    std::map<int, Player *> players;
};

#endif // SCENE_H