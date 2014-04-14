#include <iostream>
#include <iomanip>
#include <string>
#include "sgct.h"
#include "player.h"

using namespace std;

    //robber switches to cop
    void switchToCop()
    {
        cop = true;
    }

    //cop switches to robber
    void switchToRobber()
    {
        cop = false;
    }

    //the state of the player is returned
    bool isCop()
    {
        return cop;
    }

    //the direction of the movement is set depending on which button is pressed
    void setMoveDirection(int button, bool pressed)
    {
        if(button == 1)
        {
            //north
            direction = (0.0f,1.0f);
        }
        if(button == 2)
        {
            //north east
            direction = (1.0f,1.0f);
        }
        if(button == 3)
        {
            //east
            direction = (1.0f,0.0f);
        }
        if(button == 4)
        {
            //south east
            direction = (1.0f,-1.0f);
        }
        if(button == 5)
        {
            //south
            direction = (0.0f,-1.0f);
        }
        if(button == 6)
        {
            //south west
            direction = (-1.0f,-1.0f);
        }
        if(button == 7)
        {
            //kanye west
            direction = (-1.0f,0.0f);
        }
        if(button == 8)
        {
            //north west
            direction = (-1.0f,1.0f);
        }
        if(pressed == false)
        {
            //stop
            direction = (0.0f,0.0f);
        }
    }

    //The new position is calculated with direction and speed
    void movePlayer()
    {
        position += speed*direction;
    }

    //updates size and color of player when their state is changed
    void update()
    {
       
    }

    void draw()
    {

    }


