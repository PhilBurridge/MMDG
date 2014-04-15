#include "sgct.h"
#include "core.h"
#include "robberCop.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

// Decodes the received message and sends it to the process function
void Core::interpret(const char * recievedChars, int size, int clientId) {
    // Check the length of the message, between 14 and 17 chars is valid
    if(size != 0 && size >= 17 && size <= 20) {

        std::string recChars (recievedChars);

        // Define message delimiter, used to split message into components
        const char * delimiter = ";";

        // Define message keywords to search for
        std::string keywordId = "id=";
        std::string keywordBtn = "btn=";
        std::string keywordPressed = "pressed";
        std::string connected = "connected";

        // Find position of the first delimiter in the message
        firstDelimiterIndex = recChars.find(delimiter);

        // Get the id of the client, from the message
        // Find keyword and get substring from after the keyword to the delimiter
        // i.e from "id=2" get "2" as id
        std::string _id = recChars.substr(recChars.find(keywordId) + keywordId.length(), firstDelimiterIndex - keywordId.length());

        const char * temp_id = _id.c_str();
        id = atoi(temp_id);
        // Find position of the second delimiter in the message
        secondDelimiterIndex = recChars.find(delimiter, firstDelimiterIndex + 1);

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
        robberCop->process(id, action, isPressed);
    }
}

// Determines what to do with the messages
/*void Core::process (int id, int action, bool value) {
    // Inherrit from robberCopGame, todo i guess
}*/

// Sends a message to all connected clients
std::string Core::sendToAll(std::string msg) {
    
    // Add an id (in this case all) and delimiter to the message to be sent
    setMessage("id=all;" + msg);

    return msg;
}

// Sends a message to a specified client id
std::string Core::sendTo(std::string msg, int id) {
    
    // Add an id and delimiter to the message to be sent
    setMessage("id=" + std::to_string(id) + ";" + msg);
}

// Sets a message that is to be sent
void Core::setMessage(std::string msg) {
    message = msg;
}

// Gets a message that is to be sent
std::string Core::getMessage() {
    return message;
}
