#ifndef CORE_H
#define CORE_H

class Core {
public:
    Core() { };
    void interpret(const char *recievedChars, int size, int clientId);
    void process(int id, int action, bool value);
    std::String sendToAll(std::String msg);
    std::String sendTo(std::String msg, int id);
    void setMessage(std::String msg);
    std::String getMessage();

private:
    int id;
    int action;
    bool isPressed;
    size_t firstDelimiterIndex;
    size_t secondDelimiterIndex;  
    std::String message; 
};

#endif // CORE_H