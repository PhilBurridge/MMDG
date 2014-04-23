
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "sgct.h"

#ifndef CORE_H
#define CORE_H

class Core{
public:
	Core(const std::string delimiter = ";");

	std::vector<std::string> extractCommands(std::string externalInputString);
    void interpretCommand(const char *recievedChars, int size, int clientId);
    void process(int id, int action, bool value);

    void sendToAll(std::string msg);
    void sendTo(std::string msg, int id);

private:
    
    const std::string COMMAND_DELIMITER;

    size_t firstDelimiterIndex;
    size_t secondDelimiterIndex;  
};

#endif // CORE_H
