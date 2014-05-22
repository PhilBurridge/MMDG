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

const float Player::COP_SPEED = 0.1f;
const float Player::ROB_SPEED = 0.2f;
const double Player::COP_TIMER_LIMIT = 1.0;
const double Player::ROBBER_TIMER_LIMIT = 0.5;

// Player constructor
Player::Player(glm::vec2 pos, bool isCop):
DrawableObject("rob", 0.2f, 0.2f), position(pos), cop(isCop), directionIndex(0){
    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    points=0;
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
    DrawableObject::display();
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;
}

void Player::draw() const {

    DrawableObject::draw(position.x, position.y);
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
//Returns the amount of points
int Player::getPoints(){
    return points;
}

//Gets the time difference and updates the points
void Player::addPoints() {
    points++;
}
//Checks if the robber should get a point
bool Player::robberTimer() {
    robberPointTimer = clock();
    //If the time elapsed since becoming a robber is greater than the time limit
    if((robberPointTimer - startRobberTimer)/(double)(CLOCKS_PER_SEC) >= ROBBER_TIMER_LIMIT) {
        return false;
    }
    return true;
}
//Resets the time for robber
void Player::resetRobberTimer() {
    startRobberTimer = clock();
}

