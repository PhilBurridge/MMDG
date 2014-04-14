#include "sgct.h"

using namespace std;


class player {

private:

    float speed = 2.0f;
    bool cop =false;
    glm::vec2 position;
    glm::vec2 direction;
public:
	void switchToCop();
    void switchToRobber();
    bool isCop();
    void setMoveDirection(string button, bool pressed);
    void movePlayer();
    void update();
    void draw();

}