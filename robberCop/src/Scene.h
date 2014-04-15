#ifndef SCENE_H
#define SCENE_H

class Scene {
private:
    DrawableObject *background;
    int width;
    int height;
    Player *player;
    float dt; // Behövs ej?

public:
    Scene() {};
    void update();
    void updatePositions();
    void checkCollisions();
    void addPlayer();
    void draw();

    std::vector<Player *> player_vec;
};

#endif // SCENE_H