#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
#include <vector>
#include <sstream>

using namespace std;

Data::Data(int nDesks = 2) {
    numberOfDesks = nDesks;
    dataAvailability = 1;

    // Start clock (t=0)
    start = std::chrono::system_clock::now();

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

    // Allocate power metrics
    accumulatedEnergyConsumption.resize(nDesks, 0);
    accumulatedComfortError.resize(nDesks, 0);
    accumulatedComfortFlicker.resize(nDesks, 0);
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

void Data::setCurrentPwmAtDesk(float value, int desk){
    
    desk = desk - 1;

    // Insert new measured value at beggining of appliedPwm vector
    appliedPwm[desk].insert(appliedPwm[desk].begin(), value);

    /*for(int i = 0; i <  appliedPwm[desk].size(); i++){
        cout << "Value in position " << i << ": " << appliedPwm[desk][i] << endl;
    }*/

    // If vector has more than 10 elements remove last one
    if(appliedPwm[desk].size() > 10){
        appliedPwm[desk].pop_back();
    }
}

float Data::getCurrentPwmAtDesk(int desk){
    return appliedPwm[desk][0];
}

void Data::setOccupancyAtDesk(int value, int desk) {
    if(value == 0 || value == 1){
        occupancyDesk[desk-1] = value;
    }
}

int Data::getOccupancyAtDesk(int desk){
    return occupancyDesk[desk];
}

void Data::setLuxLowerBoundAtDesk(float value, int desk) {
    if(value > 0 && value < 400){
        luxLowerBound[desk-1] = value;
    }
}

float Data::getLuxLowerBoundAtDesk(int desk){
    return luxLowerBound[desk];
}

void Data::setLuxExternalAtDesk(float value, int desk){
    if(value >= 0 && value < 100){
        luxExternal[desk-1] = value;
    }
}

float Data::getLuxExternalAtDesk(int desk){
    return luxExternal[desk];
}

void Data::setLuxControlReference(float value, int desk){
    if(value >= 0 && value <= 10){
        luxControlReference[desk-1] = value;
    }
}

float Data::getLuxControlReference(int desk) {
    return luxControlReference[desk];
}

// More complex getters
float Data::getInstantaneousPowerConsumptionAtDesk(int desk){
    return this->getCurrentPwmAtDesk(desk)/5;
}

float Data::getInstantaneousPowerConsumption(){
    float powr = 0;
    for(int j=0; j < this->numberOfDesks; j++){
        powr += this->getInstantaneousPowerConsumptionAtDesk(j);
    }
    return powr;
}

void Data::accumulateEnergy(float pwm, int desk){
    // Energy = power*time
    accumulatedEnergyConsumption[desk] += pwm/5*0.005; //Period = 0.005 secs
}

float Data::getAccumulatedEnergyAtDesk(int desk){
    return accumulatedEnergyConsumption[desk];
}

float Data::getAccumulatedEnergy(){
    float energy = 0;
    for (int j=0; j < this->numberOfDesks; j++){
        energy += this->getAccumulatedEnergyAtDesk(j);
    }
    return energy;
}

std::string Data::getElapsedTimeAtDesk(int desk){
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Elapsed time: " << diff.count() << " seconds" << std::endl;
    stringstream output;
    output << diff.count();
    string response = output.str();
    return response;
}

// Reset system
void Data::reset(){
    // Start clock (t=0)
    start = std::chrono::system_clock::now();

    // Allocate 2D matrix to store measured luxs values
    measuredLuxs.assign(numberOfDesks, vector < float >(1, 987));

    // Allocate 3D matrix to store apllied pwm values
    appliedPwm.assign(numberOfDesks, vector < float >(1, 789));
    
    // Allocate occupancy vector
    occupancyDesk.resize(numberOfDesks, 1);

    // Allocate lower bounds vector
    luxLowerBound.resize(numberOfDesks, 50);

    // Allocate external illuminance vector
    luxExternal.resize(numberOfDesks, 150);

    // Allocate illuminance control reference vector
    luxControlReference.resize(numberOfDesks, 60);
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
                    response = "d " + std::to_string(arduino+1) + " " + std::to_string(this->getCurrentPwmAtDesk(arduino));
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
                    if(request[4] == 'T') {
                        printf("Power request received\n");
                        response = "p T " + std::to_string(this->getInstantaneousPowerConsumption());
                    } else {
                        response = "p " + std::to_string(arduino+1) + " " + std::to_string(this->getInstantaneousPowerConsumptionAtDesk(arduino));
                    }
                    break;
                case 't':
                    response = "t " + std::to_string(arduino+1) + " " + this->getElapsedTimeAtDesk(arduino);
                    break;
                case 'e':
                    // ToDo: edit this
                    // getAccumulatedEnergyAtDesk(arduino)
                    // getAccumulatedEnergy()
                    
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
            this->reset();
            response = "ack";
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