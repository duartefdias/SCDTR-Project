#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>

Data::Data() {
    dataAvailability = 1;
}

int Data::readAvailability() {
    return dataAvailability;
}