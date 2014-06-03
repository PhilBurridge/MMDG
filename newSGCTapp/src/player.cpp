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

const float Player::COP_SPEED = 0.4f;
const float Player::ROB_SPEED = 0.8f;
const double Player::COP_TIMER_LIMIT = 3.0;

// Player constructor
Player::Player(glm::vec2 pos, glm::vec3 c, bool isCop, sgct::Engine * e):
DrawableSquare("rob", 0.15f, 0.15f), position(pos), cop(isCop), directionIndex(0) {

    std::cout << "Player constructor" << std::endl;
    speed = isCop ? COP_SPEED : ROB_SPEED;
    gEngine = e;
    color = c;
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
bool Player::isCop() const{
    return cop;
}

// Set the direction of Player
void Player::setMoveDirection(int d){
    assert(0 <= d && d < 9);
    directionIndex = d;
    // Sets the direction with values form the Directions vector
    direction = DIRECTIONS[d];
}

int Player::getMoveDirection() const {
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
    std::cout << "x=" << position.x << std::endl;
    std::cout << "y=" << position.y << std::endl;
    if(position.y > 3.1415f-0.13f) position.y = 3.1415f-0.13f;
    if(position.y < 0.13f)    position.y = 0.13f;
}

const glm::vec2& Player::getPosition() const {
    return position;
}

// Set the players position to a specific position
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
    DrawableSquare::display();
    
    std::cout << "pos: x=" << position.x << " y=" << position.y << std::endl;
    std::cout << "direction: x=" << direction.x << " y=" << direction.y << std::endl;
    std::cout << "isCop = " << cop << std::endl;
    std::cout << "speed = " << speed << std::endl;
}

// Draw a Player
void Player::draw() const {
    DrawableSquare::draw(position.x, position.y);
    drawName();
}

void Player::drawSpherical() const{
    DrawableSquare::drawSpherical(-radius, position.y, -position.x);
    //drawNameSpherical();
}

// Draws the name of a player on the players canvas
void Player::drawName() const{

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

void Player::drawNameSpherical() const{

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

    float r = -radius; 
    float theta = position.y; 
    float phi = -position.x;

    float x = r*glm::sin(phi)*glm::cos(theta);
    float y = r*glm::sin(phi)*glm::sin(theta);
    float z = r*glm::cos(phi);

    glm::mat4 vp = gEngine->getActiveViewProjectionMatrix();

    glm::mat4 rotateY = glm::rotate(vp, 180.0f*phi/3.1415f, glm::vec3(0,1,0));
    glm::mat4 rotateX = glm::rotate(rotateY, 180.0f*theta/3.1415f, glm::vec3(1,0,0));
    glm::mat4 mvp = glm::translate(rotateX, glm::vec3(0,0,r));


    const sgct_text::Font * font = sgct_text::FontManager::instance()->getFont("SGCTFont", font_size);
    sgct_text::print3d(font, mvp, font_color, name.c_str());

    sgct_text::print(sgct_text::FontManager::instance()->getFont
        ("SGCTFont", font_size ), 
        (x_res / 2) + (x * (x_res / 3.556f)) - (name.size() * font_size / 2.3), 
        (y_res / 2) + (y * (y_res / 2.0f)) - (getSize() * 0.5f * (y_res/2)), 
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

// Sets the cop timer to current time
void Player::resetCopTimer() {
    startCopTimer = clock();
}
