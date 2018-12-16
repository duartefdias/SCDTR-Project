#include "../headers/data.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
#include <vector>

using namespace std;

Data::Data(int nDesks = 2) {
    dataAvailability = 1;

    // Allocate vector to store measured luxs values
    for(int i = 0; i < nDesks; i++){
        vector<float> row(1, 987);
        measuredLuxs.push_back(row);
    }
    
    // Allocate occupancy vector
    occupancyDesk.resize(nDesks, 1);

}

int Data::getAvailability() {
    return dataAvailability;
}

void Data::setLastLuxValueArduino(int value, char arduino) { //arduino = {1, 2}

    // Insert new measured value at beggining of measuredLuxs vector
    measuredLuxs[arduino-1].insert(measuredLuxs[arduino-1].begin(), value);

    // If vector has more than 10 elements remove last one
    if(measuredLuxs[arduino-1].size() > 10){
        measuredLuxs[arduino-1].pop_back();
    }
}

float Data::getLastLuxValueArduino(int arduino) {
    return measuredLuxs[arduino-1][0];
}

int Data::setOccupancyAtDesk(int value, int desk) {

}

std::string Data::processRequest(char* request){
    std::string response = "";
    switch(request[0]){
        case 'g':
            switch(request[2]){
                case 'l':
                        try{
                            response = std::to_string(this->getLastLuxValueArduino(request[4]));
                        }
                        catch(int e){
                            cout << "Error: " << request[4] << endl;
                        }
                    break;
                case 'd':
                    // ToDo: edit this
                    
                    break;
                case 's':
                    // ToDo: edit this
                    
                    break;
                case 'L':
                    // ToDo: edit this
                    
                    break;
                case 'o':
                    // ToDo: edit this
                    
                    break;
                case 'r':
                    // ToDo: edit this
                    
                    break;
                case 'p':
                    // ToDo: edit this
                    
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