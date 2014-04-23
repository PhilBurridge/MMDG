#include "core.h"

Core::Core(const std::string delimiter):
COMMAND_DELIMITER(delimiter){

}

std::vector<std::string> Core::extractCommands(std::string externalInputString){
    //Declare some variables
    std::vector<std::string> command_vec;
    size_t pos = 0;
    std::string token;

    //Do the job
    while ((pos = externalInputString.find(COMMAND_DELIMITER)) != std::string::npos){
        token = externalInputString.substr(0, pos);
        command_vec.push_back(token);
        externalInputString.erase(0, pos + COMMAND_DELIMITER.length());
    }

    return command_vec;
}

// Decodes the received message and sends it to the process function
void Core::interpretCommand(const char * recievedChars, int size, int clientId) {
    // Check the length of the message, between 14 and 17 chars is valid
    if(size != 0 && size >= 17 && size <= 20) {

        int id;
        int action;
        bool isPressed;

        std::string recChars (recievedChars);

        // Define message keywords to search for
        std::string keywordId = "id=";
        std::string keywordBtn = "btn=";
        std::string keywordPressed = "pressed";
        std::string connected = "connected";

        // Find position of the first delimiter in the message
        firstDelimiterIndex = recChars.find(COMMAND_DELIMITER);

        // Get the id of the client, from the message
        // Find keyword and get substring from after the keyword to the delimiter
        // i.e from "id=2" get "2" as id
        std::string _id = recChars.substr(recChars.find(keywordId) + keywordId.length(), firstDelimiterIndex - keywordId.length());

        const char * temp_id = _id.c_str();
        id = atoi(temp_id);
        // Find position of the second delimiter in the message
        secondDelimiterIndex = recChars.find(COMMAND_DELIMITER, firstDelimiterIndex + 1);

        // Obtain action to be carried out, from the message
        // Find btn keyword and get the substring from after the keyword to the delimiter
        // i.e from "btn=1" get "1" as action
        std::string _action = recChars.substr(recChars.find(keywordBtn) + keywordBtn.length(), secondDelimiterIndex - keywordBtn.length());

        const char * temp_action = _action.c_str();
        action = atoi(temp_action);

        // Check if the message contains the word "connected"
        // Connected is repressented by the int 0
        if(recChars.compare(firstDelimiterIndex + 1, connected.length(), connected) == 0) {
            action = 0;
        }

        // Check if the message contains keyword "pressed"
        // if it does, isPressed is set to true
        // else isPressed is set to false
        if(recChars.compare(secondDelimiterIndex + 1, keywordPressed.length(), keywordPressed) == 0) {
            isPressed = true;
        } else {
            isPressed = false;
        }
        process(id, action, isPressed);
    }
}

// Determines what to do with the messages
void Core::process(int id, int action, bool value){
    //this is to be overrided by applications

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

