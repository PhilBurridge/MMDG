#include <iostream>
#include "sgct.h"
#include "player.h"
#include <vector>
#include <iostream>

using namespace std;

Player::Player(glm::vec2 pos, bool state) {
    position = pos;
    cop = state;
}

void Player::switchToCop()
{
    cop = true;
}

void Player::switchToRobber()
{
    cop = false;
}

bool Player::isCop()
{
    return cop;
}

void Player::setMoveDirection(int button, bool pressed)
{
    if(button == 1)
    {
        //north
        direction = glm::vec2(0.0f,1.0f);
        //direction.x = 0.0f;
        //direction.y = 1.0f;
    }
    if(button == 2)
    {
        //north east
        direction = glm::vec2(1.0f,1.0f);
    }
    if(button == 3)
    {
        //east
        direction = glm::vec2(1.0f,0.0f);
    }
    if(button == 4)
    {
        //south east
        direction = glm::vec2(1.0f,-1.0f);
    }
    if(button == 5)
    {
        //south
        direction = glm::vec2(0.0f,-1.0f);
    }
    if(button == 6)
    {
        //south west
        direction = glm::vec2(-1.0f,-1.0f);
    }
    if(button == 7)
    {
        //kanye west
        direction = glm::vec2(-1.0f,0.0f);
    }
    if(button == 8)
    {
        //north west
        direction = glm::vec2(-1.0f,1.0f);
    }
    if(pressed == false)
    {
        //stop
        direction = glm::vec2(0.0f,0.0f);
    }
}

void Player::movePlayer()
{
    position += speed*direction;
}

void Player::update()
{
   
}

/*void Player::draw()
{

}*/


