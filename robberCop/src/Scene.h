#ifdef SCENE_H
#define SCENE_H

class Scene {
private:
    DrawableObject background;
    int width;
    int height;
    std::vector<Player> player_vec;
    float dt; // Behövs ej?

public:
    Scene() {};
    void update();
    void updatePositions();
    void checkCollisons();
    void draw();
};

#endif // SCENE_H