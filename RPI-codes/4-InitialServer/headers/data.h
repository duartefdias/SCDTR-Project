#ifndef DATA_H
#define DATA_H

#include <string>

using namespace std;

class Data{
public:

    Data();

    int getAvailability();

    void setLastLuxValueArduino(int value, char arduino);
    std::string getLastLuxValueArduino1();
    std::string getLastLuxValueArduino2();

private:
    int dataAvailability = 0;
    int lastLuxValueArduino1 = 0;
    int lastLuxValueArduino2 = 0;
};

#endif //DATA_H