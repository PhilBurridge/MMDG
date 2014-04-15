#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

class DrawableObject {
public:
    DrawableObject() { };

    void draw(/*glm::mat4 MVP,*/ glm::vec2 position);
    void init();
    void setSize(float s);
    float getSize();

private:
    float size;
    size_t textureHandle;
};

#endif // DRAWABLEOBJECT_H