#ifndef PLAYER_H
#define PLAYER_H


//#include <vector>
//#include <iostream>
//#include <glm/glm.hpp>

#include "sgct.h"
#include "scene.h"
#include "drawableObject.h"
#include "debug.h"


//class Scene;
// ADD THSI: public DrawableObject
class Player: public DrawableObject {

private:

    float speed;
    bool cop;
    glm::vec2 position;
    glm::vec2 direction;
public:
    Player();
    Player(glm::vec2 pos, bool state);
	void switchToCop();
    void switchToRobber();
    bool isCop();
    void setMoveDirection(int button, bool pressed);
    void movePlayer();
    void update();
    glm::vec2 getPosition();
    void setPosition(glm::vec2 p);
    DrawableObject *drawableObject;

    void display() const;
    
   // void draw();
};
#endif // PLAYER_H