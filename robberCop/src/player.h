#ifndef PLAYER_H
#define PLAYER_H

class Player {

private:

    float speed = 2.0f;
    bool cop = false;
    glm::vec2 position;
    glm::vec2 direction;
public:
    Player(glm::vec2 pos, bool state);
	void switchToCop();
    void switchToRobber();
    bool isCop();
    void setMoveDirection(int button, bool pressed);
    void movePlayer();
    void update();
   // void draw();
};
#endif // PLAYER_H