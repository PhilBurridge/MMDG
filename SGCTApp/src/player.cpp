#include "player.h"

// Define the speed and directions for a players movement
const glm::vec2 Player::DIRECTIONS[] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(0.707106f, 0.707106f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.707106f, -0.707106f),
    glm::vec2(0.0f, -1.0f),
    glm::vec2(-0.707106f, -0.707106f),
    glm::vec2(-1.0f, 0.0f),
    glm::vec2(-0.707106f, 0.707106f)
};

// Player constructor
Player::Player(glm::vec2 pos, bool state):
DrawableObject("rob", 0.2f, 0.2f), position(pos), cop(state), speed(0.1f){
    std::cout << "Player constructor" << std::endl;

    display();
}

void Player::switchToCop() {
    cop = true;
    textureName = "cop";
}

void Player::switchToRobber() {
    cop = false;
    textureName = "rob";
}

bool Player::isCop() {
    return cop;
}

void Player::setMoveDirection(int d){
    assert(0 <= d && d < 9);
    direction = DIRECTIONS[d];
}

void Player::stop(){
    direction = glm::vec2(0.0f, 0.0f);
}


void Player::movePlayer(float dt) {
    position += dt*speed*direction;
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

