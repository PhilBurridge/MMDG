#include "player.h"

// Define the speed and directions for a players movement
const glm::vec2 Player::DIRECTIONS[] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(0.707106f, 0.707106f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.707106f, -0.707106f),
    glm::vec2(0.0f, -1.0f),
    glm::vec2(-0.707106f, -0.707106f),
    glm::vec2(-1.0f, 0.0f),
    glm::vec2(-0.707106f, 0.707106f)
};

const float Player::COP_SPEED = 0.4f;
const float Player::ROB_SPEED = 0.8f;
const double Player::COP_TIMER_LIMIT = 3.0;
const float Player::RADIOUS = 1.0f;

// Player constructor
Player::Player(glm::vec2 pos, bool isCop):
DrawableSquare("rob", 0.1f, 0.1f), position(pos), cop(isCop), directionIndex(0){
    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    display();
}

void Player::switchToCop() {
    cop = true;
    speed = COP_SPEED;
    textureName = "cop";
    resetCopTimer();
}

void Player::switchToRobber() {
    cop = false;
    speed = ROB_SPEED;
    textureName = "rob";
    startCopTimer = 0;
}

bool Player::isCop() {
    return cop;
}

void Player::setMoveDirection(int d){
    assert(0 <= d && d < 9);
    directionIndex = d;
    direction = DIRECTIONS[d];
}

int Player::getMoveDirection() const{
    return directionIndex;
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
    DrawableSquare::display();
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;
}

void Player::draw() const {
    DrawableSquare::draw(position.x, position.y);
}

void Player::drawSpherical() const{
    DrawableSquare::drawSpherical(-RADIOUS, position.y, -position.x);
}

// Returns false if the maximum cop time is exceded
bool Player::copTimer() {
    endCopTimer = clock();
    if((endCopTimer - startCopTimer)/(double)(CLOCKS_PER_SEC) >= COP_TIMER_LIMIT) {
        return false;
    }

    return true;
}

void Player::resetCopTimer() {
    startCopTimer = clock();
}

