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
const double Player::COP_TIMER_LIMIT = 100.0;

// Player constructor
Player::Player(glm::vec2 pos, glm::vec3 c, bool isCop, sgct::Engine * e):
DrawableObject("rob", 0.15f, 0.15f), position(pos), cop(isCop), directionIndex(0) {
    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    gEngine = e;
    color = c;
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

void Player::setMoveDirection(int d) {
    assert(0 <= d && d < 9);
    directionIndex = d;
    direction = DIRECTIONS[d];
}

int Player::getMoveDirection() const {
    return directionIndex;
}

void Player::stop() {
    direction = glm::vec2(0.0f, 0.0f);
}


void Player::movePlayer(float dt) {
    position += dt*speed*direction;
}

glm::vec2 Player::getPosition() const {
    return position;
}

void Player::setPosition(glm::vec2 p) {
    position = p;
}

void Player::setName(std::string n) {
    name = n;
}

std::string Player::getName() const {
    return name;
}

void Player::display() const {
    DrawableObject::display();
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;
}

void Player::draw() const {

    DrawableObject::draw(position.x, position.y);
}

// Draws the name of a player on the players canvas
void Player::drawName() {

    // Get the resolution of the openGL window
    float x_res = (float)gEngine->getActiveXResolution();
    float y_res = (float)gEngine->getActiveYResolution();

    // Calculate the font size depending on the screen resolution
    unsigned int font_size = gEngine->getActiveXResolution() * getSize() / 20;

    // Scale the font down if name is longer than 10 chars
    if(name.size() > 10)
        font_size = (font_size * 100 * getSize()) / name.size();

    // Set the color from the current player object
    glm::vec4 font_color(color.x, color.y, color.z, 1.0f);

    // Draw name on screen, changes dynamicly depending on screen size
    // (font type, x-pos in pixels, y-pos in pixels, glm::vec4 color, std::string text)
    sgct_text::print(sgct_text::FontManager::instance()->getFont
        ("SGCTFont", font_size ), 
        (x_res / 2) + (position.x * (x_res / 3.556f)) - (name.size() * font_size / 2.3), 
        (y_res / 2) + (position.y * (y_res / 2.0f)) - (getSize() * 0.5f * (y_res/2)), 
        font_color,
        name.c_str());
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
