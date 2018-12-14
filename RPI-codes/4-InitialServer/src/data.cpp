#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>

using namespace std;

Data::Data() {
    dataAvailability = 1;
}

int Data::getAvailability() {
    return dataAvailability;
}

void Data::setLastLuxValueArduino(int value, char arduino) { //arduino = {1, 2}
    if( arduino == '1'){
        lastLuxValueArduino1 = value;
    }
    else if(arduino == '2'){
        lastLuxValueArduino2 = value;
    }
    else {
        lastLuxValueArduino1 = 999999;
        lastLuxValueArduino2 = 999999;

    }
}

float Data::getLastLuxValueArduino1() {
    return lastLuxValueArduino1;
}

float Data::getLastLuxValueArduino2() {
    return lastLuxValueArduino2; 
}

std::string Data::processRequest(char* request){
    std::string response;
    if(request[2] == 'l'){
        if(request[4] == '1'){
            response = std::to_string(this->getLastLuxValueArduino1());
        }
        else if(request[4] == '2'){
            response = std::to_string(this->getLastLuxValueArduino2());
        }
    }
    else{
        response = "Invalid request";
    }

    return response;
}