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
    std::string response = "";
    switch(request[0]){
        case 'g':
            switch(request[2]){
                case 'l':
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'd':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 's':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'L':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'o':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'r':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'p':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 't':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'e':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'c':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                case 'v':
                    // ToDo: edit this
                    if(request[4] == '1'){
                        response = std::to_string(this->getLastLuxValueArduino1());
                    }
                    else if(request[4] == '2'){
                        response = std::to_string(this->getLastLuxValueArduino2());
                    }
                    break;
                
                default:
                    response = "Invalid request";
            }
            break;
        case 'r':
            //ToDo
            break;
        case 'b':
            //ToDo
            break;
        case 's':
            //ToDo
            break;
        default:
            response = "Invalid request";
    }
    

    return response;
}