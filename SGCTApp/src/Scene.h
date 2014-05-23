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
    // Constructor
    Scene();

    // Updates the Scene
    void update(float dt);

    // Checks if a collision occurs
    void checkCollisions();

    // Handels Players
    void addPlayer(int id, Player *);
    bool removePlayer(int id);
    Player * getPlayer(int id);
    unsigned int getNumberOfPlayers();

    // Draws the Scene
    void draw();
    
private:
    // The background
    DrawableObject *background;

    int width;
    int height;

    // Players in the Scene
    std::map<int, Player *> players;
};

#endif // SCENE_H