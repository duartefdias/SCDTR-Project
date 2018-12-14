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

std::string Data::getLastLuxValueArduino1() {
    char valueCharArray[100]; 
    sprintf(valueCharArray,"%f", lastLuxValueArduino1); 
    return std::string(valueCharArray); 
}

std::string Data::getLastLuxValueArduino2() {
    char valueCharArray[100];
    sprintf(valueCharArray,"%f", lastLuxValueArduino2); 
    return std::string(valueCharArray); 
}