#include "player.h"
#include "debug.h"

Player::Player(glm::vec2 pos, bool state):
    position(pos),
    cop(state),
    speed(0.1f)
{
    debug
    drawableObject = new DrawableObject();
    debug
};

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

void Player::setMoveDirection(int button, bool pressed) {
    if(button == 1) {
        //north
        direction = glm::vec2(0.0f,1.0f);
        //direction.x = 0.0f;
        //direction.y = 1.0f;
    }
    if(button == 2) {
        //north east
        direction = glm::vec2(1.0f,1.0f);
    }
    if(button == 3) {
        //east
        direction = glm::vec2(1.0f,0.0f);
    }
    if(button == 4) {
        //south east
        direction = glm::vec2(1.0f,-1.0f);
    }
    if(button == 5) {
        //south
        direction = glm::vec2(0.0f,-1.0f);
    }
    if(button == 6) {
        //south west
        direction = glm::vec2(-1.0f,-1.0f);
    }
    if(button == 7) {
        //kanye west
        direction = glm::vec2(-1.0f,0.0f);
    }
    if(button == 8) {
        //north west
        direction = glm::vec2(-1.0f,1.0f);
    }
    if(pressed == false) {
        //stop
        direction = glm::vec2(0.0f,0.0f);
    }
}

void Player::movePlayer() {
    position += speed*direction;
}

void Player::update() {
   
}

glm::vec2 Player::getPosition() {
    return position;
}

void Player::setPosition(glm::vec2 p) {
    position = p;
}

void Player::display() const{
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;

}

/*void Player::draw()
{

}*/


