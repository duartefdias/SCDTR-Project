#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
#include <vector>
#include <sstream>

using namespace std;

Data::Data(int nDesks = 2) {
    numberOfDesks = nDesks;
    dataAvailability = 1;

    // Allocate vector to store measured luxs values
    measuredLuxs.assign(nDesks, vector < float >(1, 987));
    
    // Allocate occupancy vector
    occupancyDesk.resize(nDesks, 1);

    // Allocate lower bounds vector
    luxLowerBound.resize(nDesks, 50);

    // Allocate external illuminance vector
    luxExternal.resize(nDesks, 150);

    // Allocate illuminance control reference vector
    luxControlReference.resize(nDesks, 60);

}

int Data::getAvailability() {
    return dataAvailability;
}

// Basic getters and setters
void Data::setLastLuxValueArduino(int value, char arduino) { //arduino = {1, 2}

    // Convert arduino char into int
    stringstream strValue;
    strValue << arduino;
    int arduinoInt;
    strValue >> arduinoInt;
    arduinoInt = arduinoInt - 1; // Because of indexes

    // Insert new measured value at beggining of measuredLuxs vector
    measuredLuxs[arduino].insert(measuredLuxs[arduino].begin(), value);

    // If vector has more than 10 elements remove last one
    if(measuredLuxs[arduino].size() > 10){
        measuredLuxs[arduino].pop_back();
    }
}

float Data::getLastLuxValueArduino(int arduino) {
    return measuredLuxs[arduino][0];
}

void Data::setOccupancyAtDesk(int value, int desk) {
    if(value == 0 || value == 1){
        occupancyDesk[desk] = value;
    }
}

int Data::getOccupancyAtDesk(int desk){
    return occupancyDesk[desk];
}

void Data::setLuxLowerBoundAtDesk(float value, int desk) {
    if(value > 0 && value < 400){
        luxLowerBound[desk] = value;
    }
}

int Data::getLuxLowerBoundAtDesk(int desk){
    return luxLowerBound[desk];
}

void Data::setLuxExternalAtDesk(float value, int desk){
    if(value > 0 && value < 1000){
        luxExternal[desk] = value;
    }
}

int Data::getLuxExternalAtDesk(int desk){
    return luxExternal[desk];
}

void Data::setLuxControlReference(float value, int desk){
    if(value > 0 && value < 1000){
        luxControlReference[desk] = value;
    }
}

int Data::getLuxControlReference(int desk) {
    return luxControlReference[desk];
}

// More complex getters
float Data::getInstantaneousPowerConsumptionAtDesk(int desk){
    //do something
}

std::string Data::processRequest(char* request){
    std::string response = "";

    // Converting arduino value from const char to int
    stringstream strValue;
    strValue << request[4];
    int arduino;
    strValue >> arduino;
    if(arduino < 1 || arduino > numberOfDesks){return response = "Invalid request!";}
    arduino = arduino - 1; // Because of indexes
    // End of convertion

    switch(request[0]){
        case 'g':
            switch(request[2]){
                case 'l':
                    response = std::to_string(this->getLastLuxValueArduino(arduino));
                    break;
                case 'd':
                    // ToDo: edit this
                    
                    break;
                case 's':
                    response = std::to_string(this->getOccupancyAtDesk(arduino));
                    break;
                case 'L':
                    response = std::to_string(this->getLuxLowerBoundAtDesk(arduino));
                    break;
                case 'o':
                    response = std::to_string(this->getLuxExternalAtDesk(arduino));
                    break;
                case 'r':
                    response = std::to_string(this->getLuxControlReference(arduino));
                    break;
                case 'p':
                    // ToDo: edit this
                    response = std::to_string(this->getInstantaneousPowerConsumptionAtDesk(arduino));
                    break;
                case 't':
                    // ToDo: edit this
                    
                    break;
                case 'e':
                    // ToDo: edit this
                    
                    break;
                case 'c':
                    // ToDo: edit this
                    
                    break;
                case 'v':
                    // ToDo: edit this
                    
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