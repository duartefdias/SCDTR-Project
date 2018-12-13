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

char* Data::getLastLuxValueArduino1() {
    char value[10] = std::to_string(lastLuxValueArduino1);
    return value;
}

char* Data::getLastLuxValueArduino2() {
    char value[10] = std::to_string(lastLuxValueArduino2);
    return value;
}