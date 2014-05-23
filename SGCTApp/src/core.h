
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//#include <ctime>
#include "sgct.h"
//#include "robberCop.h"

#ifndef CORE_H
#define CORE_H

class Core{
public:
	Core(sgct::Engine * gEngine);

	//Handles communication from clients
	void handleExternalInput(const char * recievedChars, int size, int clientId);
	virtual void process(int id, std::string variable, std::string value);

	//Handles communication to clients
	void sendToAll(std::string msg);
    void sendTo(std::string msg, int id);


    //						  	CMD_DELIMITER
    //			  				 	 |
    //			  					 v
    //	Example: "id=2 var=btn1 val=1;"
    //				  ^		   ^
    //				  |   	   |
    //		   ARG_DELIMITER   ARG_DELIMITER

    static const std::string CMD_DELIMITER;
	static const std::string ARG_DELIMITER;


	//     cmd_args[0] ...  cmd_args[2]
    //            |     |        |
    //            v     v        v
    //	Example: "id=2 var=btn1 val=1;"

	std::vector<std::string> cmd_args;

    //A pointer to the SGCT engine
    sgct::Engine * gEngine;

	//Benchmarking
	void startBenchmark();
	void stopBenchmark();
	void getPingStats(double &min, double &max, double &avg);
	void printPingStats();

private:

	// for converting numbers to strings
	//std::ostringstream convert;
    
    //virtual void handleMessage(int id, std::string action, std::string value);
	
    //extracts commands delimited by CMD_DELIMITER from a string
    std::vector<std::string> extractCommands(std::string externalInputString);

    //Analyzes commands for "arguments" separated by ARG_DELIMITER
    bool analyzeCommand(std::string command, int *id, std::string *variable, std::string *value);

    /* BENCHMARKING VARIABLES */
	clock_t startClock;
	clock_t endClock;
	const std::string PING_MESSAGE = "ping";
	std::vector<double> pingResponses;
};

#endif // CORE_H
