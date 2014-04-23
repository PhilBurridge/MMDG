#include "core.h"

Core::Core(){
    cmd_delimiter = ";";
    arg_delimiter = " ";
}

void Core::handleExternalInput(const char * recievedChars, int size, int clientId){
    std::string externalInputString(recievedChars);
    std::vector<std::string> command_vec = extractCommands(externalInputString);

    int id;
    std::string variable = "";
    std::string value = "";
    for (int i = 0; i < command_vec.size(); ++i){
        analyzeCommand(command_vec[i], id, variable, value);
    }

    process(id,variable,value);
}


std::vector<std::string> Core::extractCommands(std::string externalInputString){
    std::cout << "external input string: " << externalInputString << std::endl;

    std::vector<std::string> command_vec;
    size_t delimiter_pos = 0;
    std::string token;

    //Extract commands and put them into a vector
    while ((delimiter_pos = externalInputString.find(cmd_delimiter)) != std::string::npos){
        token = externalInputString.substr(0, delimiter_pos);
        command_vec.push_back(token + arg_delimiter); //Adding the arg_delimiter makes analyzing the commands easier
        externalInputString.erase(0, delimiter_pos + cmd_delimiter.length());
    }

    return command_vec;
}

// Decodes the received command and sends it to the process function
bool Core::analyzeCommand(std::string command, int &id, std::string &var, std::string &val) {
    
    const size_t n_find_keys = 3;
    std::string keys[] = {"id=", "var=", "val="};
    std::string found_values[n_find_keys];
    
    size_t delimiter_pos;

    for (int i = 0; i < n_find_keys; ++i){
        std::cout << "to be analyzed: " << command << std::endl;

        //Make sure that keys match
        if(command.substr(0,keys[i].length()) != keys[i]){
            std::cout << "ERROR! expected \"" << keys[i] << "\" but recieved \"" << command.substr(0,keys[i].length()) << std::endl;
            return false;
        }
        command.erase(0,keys[i].length());

        //Grabs everything after key until arg_delimiter is found
        delimiter_pos = command.find(arg_delimiter);
        if(delimiter_pos == std::string::npos){
            std::cout << "ERROR! Couldn't find any delimiter for key \"" << keys[i] << "\""<< std::endl;
            return false;
        }
        found_values[i] = command.substr(0,delimiter_pos);
        command.erase(0,delimiter_pos + arg_delimiter.length());
    }

    id = atoi(found_values[0].c_str());
    var = found_values[1];
    val = found_values[2];

    return true;
}

// Determines what to do with the messages
void Core::process(int id, std::string variable, std::string value){
    //this method is to be overridden by applications like RobberCop

    std::cout << "Input data to be processed: " << std::endl;
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

