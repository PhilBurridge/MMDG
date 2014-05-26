#ifndef PLAYER_H
#define PLAYER_H


//#include <vector>
//#include <iostream>
//#include <glm/glm.hpp>
#include <ctime>
#include "sgct.h"
#include "scene.h"
#include "drawableObject.h"
#include "debug.h"


class Player: public DrawableObject {

public:
    Player();
    Player(glm::vec2 pos, 
        bool state);

	void switchToCop();
    void switchToRobber();
    bool isCop();
    bool isMortal();

    void setMoveDirection(int d);
    int getMoveDirection() const;
    void stop();

    void movePlayer(float dt);

    glm::vec2 getPosition() const;
    void setPosition(glm::vec2 p);

    void display() const;

    void draw() const;
    
    bool copTimer();

    void resetCopTimer();
    
    enum DirectionEnum{
        STOP = 0,
        NORTH, 
        NORTH_EAST, 
        EAST, 
        SOUTH_EAST, 
        SOUTH, 
        SOUTH_WEST, 
        WEST, 
        NORTH_WEST
    } DIRECTION;



private:

    float speed;
    bool cop;
    bool blinking;
    bool drawIt;
    bool mortal;
    int directionIndex;
    glm::vec2 position;
    glm::vec2 direction;

    clock_t startCopTimer;
    clock_t endCopTimer;
    static const double COP_TIMER_LIMIT;

    static const float COP_SPEED;
    static const float ROB_SPEED;
    
    static const glm::vec2 DIRECTIONS[];
};

#endif // PLAYER_H