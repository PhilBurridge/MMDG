#include "player.h"


Player::Player(glm::vec2 pos, bool state):
DrawableObject("box.png", 0.2f, 0.2f), position(pos), cop(state), speed(0.1f){
    debug
    display();
    //sgct::TextureManager::instance()->loadTexure(
    //    textureHandle, "player", "./textures/" + texture, true);
    debug
    //init();
    display();
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

void Player::setMoveDirection(int button, bool pressed) {
    if(button == 1) {
        //north
        direction = glm::vec2(0.0f,1.0f);
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

void Player::movePlayer(float dt) {
    position += dt*speed*direction;
}

void Player::update(float dt) {
   
}

glm::vec2 Player::getPosition() const{
    return position;
}

void Player::setPosition(glm::vec2 p) {
    position = p;
}

void Player::display() const{
    DrawableObject::display();
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;

}

void Player::draw() const {
    DrawableObject::draw(position.x, position.y);
}


