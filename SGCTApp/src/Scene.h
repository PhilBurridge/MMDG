#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>

#include "debug.h"
#include "sgct.h"
#include "robberCop.h"
#include "player.h"
#include "drawableObject.h"

class Player;
//class DrawableObject;

class Scene {
private:
    DrawableObject *background;
    int width;
    int height;
    //Player *player;
    float dt; // Behövs ej?
    size_t backgroundhandle;

public:
    Scene();
    void update();
    void updatePositions();
    void checkCollisions();
    void addPlayer(Player *);
    void draw();
    void init();

    std::vector<Player *> player_vec;
};

#endif // SCENE_H