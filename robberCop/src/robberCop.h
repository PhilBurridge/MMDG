#ifdef ROBBERCOP_H
#define ROBBERCOP_H

class RobberCop {
private:
    Scene *scene;

public:
    RobberCop() { };
    void process(int id, int action, bool value);

};

#endif // ROBBERCOP_H