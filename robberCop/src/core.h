#ifndef CORE_H
#define CORE_H

class Core {
public:
    Core() { };
    void interpret(const char *recievedChars, int size, int clientId);
    //void process(int id, int action, bool value);
    std::string sendToAll(std::string msg);
    std::string sendTo(std::string msg, int id);
    void setMessage(std::string msg);
    std::string getMessage();

private:
    int id;
    int action;
    bool isPressed;
    size_t firstDelimiterIndex;
    size_t secondDelimiterIndex;  
    std::string message; 
};

#endif // CORE_H