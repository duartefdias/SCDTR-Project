#include "../headers/i2cFunctions.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>

I2cFunctions::I2cFunctions() {
    i2cAvailability = 1;
}

int I2cFunctions::getAvailability() {
    return i2cAvailability;
}