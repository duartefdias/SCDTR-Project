#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
#include <vector>
#include <sstream>

using namespace std;

Data::Data(int nDesks = 2) {
    numberOfDesks = nDesks;
    dataAvailability = 1;

    // Allocate 2D matrix to store measured luxs values
    measuredLuxs.assign(nDesks, vector < float >(1, 987));

    // Allocate 3D matrix to store apllied pwm values
    appliedPwm.assign(nDesks, vector < float >(1, 789));
    
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
void Data::setLastLuxValueArduino(float value, int arduino) { //arduino = {1, 2}

    arduino = arduino - 1;

    // Insert new measured value at beggining of measuredLuxs vector
    measuredLuxs[arduino].insert(measuredLuxs[arduino].begin(), value);

    //Debugging messages
    /*for(int i = 0; i <  measuredLuxs[arduino].size(); i++){
        cout << "Value in position " << i << ": " << measuredLuxs[arduino][i] << endl;
    }*/

    // If vector has more than 10 elements remove last one
    if(measuredLuxs[arduino].size() > 10){
        measuredLuxs[arduino].pop_back();
    }
}

float Data::getLastLuxValueArduino(int arduino) {
    std::cout << "Returned lastest lux value: " << measuredLuxs[arduino][0] << std::endl;
    return measuredLuxs[arduino][0];
}

void Data::setcurrentPwmAtDesk(float value, int desk){
    
    desk = desk - 1;

    // Insert new measured value at beggining of appliedPwm vector
    appliedPwm[desk].insert(appliedPwm[desk].begin(), value);

    for(int i = 0; i <  appliedPwm[desk].size(); i++){
        cout << "Value in position " << i << ": " << appliedPwm[desk][i] << endl;
    }

    // If vector has more than 10 elements remove last one
    if(appliedPwm[desk].size() > 10){
        appliedPwm[desk].pop_back();
    }
}

float Data::getcurrentPwmAtDesk(int desk){
    return appliedPwm[desk][0];
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
    if(arduino < 1 || arduino > numberOfDesks){return response = "Invalid request!                                 ";}
    arduino = arduino - 1; // Because of indexes
    // End of convertion

    switch(request[0]){
        case 'g':
            switch(request[2]){
                case 'l':
                    response = "l " + std::to_string(arduino+1) + " " + std::to_string(this->getLastLuxValueArduino(arduino));
                    cout << "Response: " << response << endl;
                    break;
                case 'd':
                    response = "d " + std::to_string(arduino+1) + " " + std::to_string(this->getcurrentPwmAtDesk(arduino));
                    break;
                case 's':
                    response = "s " + std::to_string(arduino+1) + " " + std::to_string(this->getOccupancyAtDesk(arduino));
                    break;
                case 'L':
                    response = "L " + std::to_string(arduino+1) + " " + std::to_string(this->getLuxLowerBoundAtDesk(arduino));
                    break;
                case 'o':
                    response = "o " + std::to_string(arduino+1) + " " + std::to_string(this->getLuxExternalAtDesk(arduino));
                    break;
                case 'r':
                    response = "r " + std::to_string(arduino+1) + " " + std::to_string(this->getLuxControlReference(arduino));
                    break;
                case 'p':
                    // ToDo: edit this
                    if(request[4] == 'T')
                        response = "p " + std::to_string(arduino+1) + " " + std::to_string(this->getInstantaneousPowerConsumptionAtDesk(arduino));
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
            switch(request[2]){
                case 'i':
                    // Get all lux values in last minute
                    break;
                case 'd':
                    // Gett all pwm values in last minute
                    break;
            }
            break;
        case 's':
            //ToDo
            break;
        default:
            response = "Invalid request";
    }
    
    response = response + "                                  ";
    return response;
}