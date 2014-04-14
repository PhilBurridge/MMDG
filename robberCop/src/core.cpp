#include "sgct.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "core.h"

// Decodes the received message and sends it to the process function
void Core::interpret(const char * recievedChars, int size, int clientId) {
    // Check the length of the message, between 14 and 17 chars is valid
    if(size != 0 && size >= 17 && size <= 20) {

        // Define message delimiter, used to split message into components
        std::String delimiter = ";";

        // Define message keywords to search for
        std::String keywordId = "id=";
        std::String keywordBtn = "btn=";
        std::String keywordPressed = "pressed";
        std::String connected = "connected";

        // Find position of the first delimiter in the message
        firstDelimiterIndex = recievedChars.find(delimiter);

        // Get the id of the client, from the message
        // Find keyword and get substring from after the keyword to the delimiter
        // i.e from "id=2" get "2" as id
        id = atoi(recievedChars.substr(recievedChars.find(keywordId) + keywordId.length(), firstDelimiterIndex - keywordId.length()).c_str);

        // Find position of the second delimiter in the message
        secondDelimiterIndex = recievedChars.find(delimiter, firstDelimiterIndex + 1);

        // Obtain action to be carried out, from the message
        // Find btn keyword and get the substring from after the keyword to the delimiter
        // i.e from "btn=1" get "1" as action
        action = atoi(recievedChars.substr(recievedChars.find(keywordBtn) + keywordBtn.length(), secondDelimiterIndex).c_str);

        // Check if the message contains the word "connected"
        // Connected is repressented by the int 0
        if(strncmp(action + firstDelimiterIndex + 1, connected, connected.length())) {
            action = 0;
        }

        // Check if the message contains keyword "pressed"
        // if it does, isPressed is set to true
        // else isPressed is set to false
        if(recievedChars.strncmp(recievedChars + secondDelimiterIndex + 1, keywordPressed, keywordPressed.length())) {
            isPressed = true;
        } else {
            isPressed = false;
        }
        process(id, action, isPressed);
    }
}

// Determines what to do with the messages
void Core::process (int id, int action, bool value) {
    // Inherrit from robberCopGame, todo i guess
}

// Sends a message to all connected clients
std::String Core::sendToAll(std::String msg) {
    
    // Add an id (in this case all) and delimiter to the message to be sent
    setMessage("id=all;" + msg);

    return msg;
}

// Sends a message to a specified client id
std::String Core::sendTo(std::String msg, int id) {
    
    // Add an id and delimiter to the message to be sent
    setMessage("id=" + id + ";" + msg);
}

// Sets a message that is to be sent
void Core::setMessage(std::String msg) {
    message = msg;
}

// Gets a message that is to be sent
std::String getMessage() {
    return message;
}
