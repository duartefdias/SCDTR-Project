#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>

Data::Data() {
    dataAvailability = 1;
}

int Data::getAvailability() {
    return dataAvailability;
}

void Data::setLastLuxValueArduino1(int value) {
    lastLuxValueArduino1 = value;
}

int Data::getLastLuxValueArduino1() {
    return lastLuxValueArduino1;
}

void Data::setLastLuxValueArduino2(int value) {
    lastLuxValueArduino2 = value;
}

int Data::getLastLuxValueArduino2() {
    return lastLuxValueArduino2;
}