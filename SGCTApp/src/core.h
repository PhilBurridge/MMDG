
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "sgct.h"

#ifndef CORE_H
#define CORE_H

class Core{
public:
	Core(const std::string delimiter);

    void interpret(const char *recievedChars, int size, int clientId);
    void process(int id, int action, bool value);

    void sendToAll(std::string msg);
    void sendTo(std::string msg, int id);

private:
    
    std::string COMMAND_DELIMITER;

    size_t firstDelimiterIndex;
    size_t secondDelimiterIndex;  
};

#endif // CORE_H
