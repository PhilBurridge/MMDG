#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

class DrawableObject {
public:
    DrawableObject() { };

    void draw(/*glm::mat4 MVP,*/ float position);
    void init();
private:
    float size;
    size_t textureHandle;
};

#endif // DRAWABLEOBJECT_H