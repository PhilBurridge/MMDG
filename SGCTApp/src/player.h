#ifndef PLAYER_H
#define PLAYER_H


//#include <vector>
//#include <iostream>
//#include <glm/glm.hpp>

#include "sgct.h"
#include "scene.h"
#include "drawableObject.h"
#include "debug.h"

class Player: public DrawableObject {

public:
    Player();
    Player(glm::vec2 pos, bool state);

	void switchToCop();
    void switchToRobber();
    bool isCop();
    void setMoveDirection(int button, bool pressed);
    void movePlayer(float dt);
    void update(float dt);
    
    glm::vec2 getPosition() const;
    void setPosition(glm::vec2 p);

    void display() const;
    
    void draw() const;


private:

    float speed;
    bool cop;
    glm::vec2 position;
    glm::vec2 direction;
    
};
#endif // PLAYER_H