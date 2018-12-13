#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>

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

int Data::getLastLuxValueArduino1() {
    return lastLuxValueArduino1;
}

int Data::getLastLuxValueArduino2() {
    return lastLuxValueArduino2;
}