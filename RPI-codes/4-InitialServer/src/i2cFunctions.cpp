#include "../headers/i2cFunctions.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
//#include <unistd.h>

using namespace std;

I2cFunctions::I2cFunctions() {
    i2cAvailability = 1;
}

int I2cFunctions::getAvailability() {
    return i2cAvailability;
}

void I2cFunctions::readLoop() {
    for(int i = 0; i<11; i++){
        std::cout << "i2c thread: " << i << std::endl;
        //usleep(2000);
    }
}