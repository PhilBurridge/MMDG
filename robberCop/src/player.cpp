#include <iostream>
#include <iomanip>
#include <string>
#include "sgct.h"
#include "player.h"

using namespace std;


    void switchToCop()
    {
        cop = true;
    }

    void switchToRobber()
    {
        cop = false;
    }

    bool isCop()
    {
        return cop;
    }

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

    void movePlayer()
    {
        position += speed*direction;
    }

    void update()
    {
       
    }

    void draw()
    {

    }


