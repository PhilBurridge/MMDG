#include "core.h"

Core::Core(){

    //The code below definds that commands must have this exact format:
    // "id=<id> var=<var> val=<val>;"
    cmd_args.push_back("id=");
    cmd_args.push_back("var=");
    cmd_args.push_back("val=");
    
}

const std::string Core::ARG_DELIMITER = " ";
const std::string Core::CMD_DELIMITER = ";";

void Core::handleExternalInput(const char * recievedChars, int size, int clientId){
    std::string externalInputString(recievedChars);
    std::vector<std::string> command_vec = extractCommands(externalInputString);

    int id;
    std::string variable = "";
    std::string value = "";
    for (int i = 0; i < command_vec.size(); ++i){

        //Pass pointer to variables. This method will update the variables values.
        if(analyzeCommand(command_vec[i], &id, &variable, &value)){

            //Now do something with the extracted information
            process(id,variable,value);
        }    
    }
}


std::vector<std::string> Core::extractCommands(std::string externalInputString){
    std::cout << "external input string: " << externalInputString << std::endl;

    std::vector<std::string> command_vec;
    size_t delimiter_pos = 0;
    std::string token;

    //Extract commands and put them into a vector
    while ((delimiter_pos = externalInputString.find(CMD_DELIMITER)) != std::string::npos){
        token = externalInputString.substr(0, delimiter_pos);
        command_vec.push_back(token + ARG_DELIMITER); //Adding the ARG_DELIMITER makes analyzing the commands easier
        externalInputString.erase(0, delimiter_pos + CMD_DELIMITER.length());
    }
    if(externalInputString.length() > 0){
        std::cout << "WARNING! found command with no command delimiter" << std::endl;
    }

    return command_vec;
}

// Decodes the received command and sends it to the process function
bool Core::analyzeCommand(std::string command, int *id, std::string *var, std::string *val) {
    
    std::cout << "analyzing: " << command << std::endl;

    const size_t n_cmd_args = 3;
    std::string extracted_values[n_cmd_args];
    
    size_t delimiter_pos;

    //Loop through all cmd_args
    for (int i = 0; i < n_cmd_args; ++i){
        //std::cout << "to be analyzed: " << command << std::endl;

        //Make sure that cmd_args match
        if(command.substr(0,cmd_args[i].length()) != cmd_args[i]){
            std::cout << "ERROR! Bad command argument: expected \"" << cmd_args[i] << "\" but recieved \"" << command.substr(0,cmd_args[i].length()) << "\"" << std::endl;
            return false;
        }
        command.erase(0,cmd_args[i].length());

        //Grabs everything after key until ARG_DELIMITER is found
        delimiter_pos = command.find(ARG_DELIMITER);
        if(delimiter_pos == std::string::npos){
            std::cout << "ERROR! Couldn't find any delimiter for command argument \"" << cmd_args[i] << "\""<< std::endl;
            return false;
        }
        extracted_values[i] = command.substr(0,delimiter_pos);
        command.erase(0,delimiter_pos + ARG_DELIMITER.length());
    }

    //Bind the pointers to the extracted values
    *id = atoi(extracted_values[0].c_str());
    *var = extracted_values[1];
    *val = extracted_values[2];

    return true;
}

// Determines what to do with the messages
void Core::process(int id, std::string variable, std::string value){
    //this method is to be overridden by applications like RobberCop

    std::cout << "Extracted input data: " << std::endl;
    std::cout << "id = " << id << std::endl;
    std::cout << "variable = " << variable << std::endl;
    std::cout << "value = " << value << std::endl;
    std::cout << std::endl;

}


// Sends a message to all connected clients
void Core::sendToAll(std::string msg) {
    
    // Add an id (in this case all) and delimiter to the message to be sent
}

// Sends a message to a specified client id
void Core::sendTo(std::string msg, int id) {
    
    // Add an id and delimiter to the message to be sent
    //setMessage("id=" + std::to_string(id) + ";" + msg);
}

