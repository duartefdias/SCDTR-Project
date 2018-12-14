#ifndef DATA_H
#define DATA_H

#include <string>

using namespace std;

class Data{
public:

    Data();

    int getAvailability();

    void setLastLuxValueArduino(int value, char arduino);
    float getLastLuxValueArduino1();
    float getLastLuxValueArduino2();

    std::string processRequest(char* request);

private:
    int dataAvailability = 0;
    float lastLuxValueArduino1 = 9.69;
    float lastLuxValueArduino2 = 8.69;
};

#endif //DATA_H