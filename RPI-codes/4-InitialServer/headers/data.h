#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>

using namespace std;

class Data{
public:

    Data(int nDesks);

    int getAvailability();

    void setLastLuxValueArduino(int value, char arduino);
    float getLastLuxValueArduino(int arduino);
    int setOccupancyAtDesk(int value, int desk);

    std::string processRequest(char* request);

private:
    int numberOfDesks = 0;
    int dataAvailability = 0;
    vector< vector<float> > measuredLuxs;
    std::vector <int> occupancyDesk;
};

#endif //DATA_H