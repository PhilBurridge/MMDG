
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
	Core();

	//Handles communication from clients
	void handleExternalInput(const char * recievedChars, int size, int clientId);
	void process(int id, std::string variable, std::string value);

	//Handles communication to clients
	void sendToAll(std::string msg);
    void sendTo(std::string msg, int id);

private:
	std::string cmd_delimiter;
	std::string arg_delimiter;
	std::vector<std::string> cmd_args;

    std::vector<std::string> extractCommands(std::string externalInputString);
    bool analyzeCommand(std::string command, int &id, std::string &variable, std::string &value);

    size_t firstDelimiterIndex;
    size_t secondDelimiterIndex;
};

#endif // CORE_H
