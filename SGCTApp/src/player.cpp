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
const double Player::COP_TIMER_LIMIT = 30.0;

// Player constructor
Player::Player(glm::vec2 pos, bool isCop):
DrawableObject("rob", 0.2f, 0.2f), position(pos), cop(isCop), directionIndex(0){
    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    display();
}

void Player::switchToCop() {
    cop = true;
    speed = COP_SPEED;
    textureName = "cop";
    resetCopTimer();
    //when a cop is created the blinking sequence is initiated
    blinking = true;
    mortal = false;
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
bool Player::isMortal() {
    return mortal;
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
    //if the blinking sequence is initiated and we should draw it it is drawn, no shit sherlock
    if(blinking == true && drawIt == true){
        DrawableObject::draw(position.x, position.y);
    }
    else if (blinking == false){
        DrawableObject::draw(position.x, position.y);
    }
}

// Returns false if the maximum cop time is exceded
bool Player::copTimer() {
    endCopTimer = clock();
    double currTime = (endCopTimer - startCopTimer)/(double)(CLOCKS_PER_SEC);
    // if the current time is more than the cop limit, cop becomes a thief
    if(currTime > COP_TIMER_LIMIT) {
        return false;
    }
    //Blinking initiated for first 5 seconds
    else if(blinking && (currTime < 0.5)) {
        drawIt = false;
    }
    else if(blinking && (currTime < 1)) {
        drawIt = true;
    }
    else if(blinking && (currTime < 1.5)) {
        drawIt = false; 
    }
    else if(blinking && (currTime < 2)) {
        drawIt = true; 
    }
    else if(blinking && (currTime < 2.5)) {
        drawIt = false; 
    }
    else if(blinking && (currTime < 3)) {
        drawIt = true; 
    }
    else if(blinking && (currTime < 3.5)) {
        drawIt = false; 
    }
    
    // if (currTime < 5){
    //     if(blinking && (currTime < i) && ((i % 2) == 0)) {
    //         drawIt = false;
    //         i--;
    //         std::cout << "sBAJSBAJSBAJ " << std::endl;
    //     }
    //     else if(blinking && (currTime < i)) {
    //         drawIt = true;
    //         i--;
    //         std::cout << "snoppsnopp" << std::endl; 
    //     }    
    // } 
    if(blinking && (currTime > 3.5)) {
        blinking = false;
        mortal = true;
    }

    return true;
}

void Player::resetCopTimer() {
    startCopTimer = clock();
}
