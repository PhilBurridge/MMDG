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

// Constant variables for a Player
const float Player::COP_SPEED = 0.1f;
const float Player::ROB_SPEED = 0.2f;
const double Player::COP_TIMER_LIMIT = 1.0;

// Player constructor
Player::Player(glm::vec2 pos, bool isCop):
DrawableObject("rob", 0.2f, 0.2f), position(pos), cop(isCop), directionIndex(0){
    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    display();
}

// Switch from Robber to Cop
void Player::switchToCop() {
    cop = true;
    speed = COP_SPEED;
    textureName = "cop";
    resetCopTimer();
}

// Switch from Cop to Robber
void Player::switchToRobber() {
    cop = false;
    speed = ROB_SPEED;
    textureName = "rob";
    startCopTimer = 0;
}

// Check if Player is a cop
bool Player::isCop() {
    return cop;
}

// Set the direction of Player
void Player::setMoveDirection(int d){
    assert(0 <= d && d < 9);
    directionIndex = d;
    // Sets the direction with values form the Directions vector
    direction = DIRECTIONS[d];
}

// Returns the direction of a player
int Player::getMoveDirection() const{
    return directionIndex;
}

// Stops a player
void Player::stop(){
    // Sets the player direction, to 0, to stop the player
    direction = glm::vec2(0.0f, 0.0f);
}

// Multiplies the player direction with its speed
void Player::movePlayer(float dt) {
    position += dt*speed*direction;
}

// Returns the current position
glm::vec2 Player::getPosition() const{
    return position;
}

// Set the players position to a specific position
void Player::setPosition(glm::vec2 p) {
    position = p;
}

// Displays the properties of Player
void Player::display() const{
    DrawableObject::display();
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;
}

// Draw a Player
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

// Sets the cop timer to current time
void Player::resetCopTimer() {
    startCopTimer = clock();
}

