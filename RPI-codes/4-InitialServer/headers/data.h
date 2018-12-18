#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <time.h>
#include <chrono>

using namespace std;

class Data{
public:

    // Constructor
    Data(int nDesks);

    int getAvailability();

    // Basic getters and setters
    void setLastLuxValueArduino(float value, int arduino);
    float getLastLuxValueArduino(int arduino);
    void setcurrentPwmAtDesk(float value, int arduino);
    float getcurrentPwmAtDesk(int arduino);
    void setOccupancyAtDesk(int value, int desk);
    int getOccupancyAtDesk(int desk);
    void setLuxLowerBoundAtDesk(float value, int desk);
    int getLuxLowerBoundAtDesk(int desk);
    void setLuxExternalAtDesk(float value, int desk);
    int getLuxExternalAtDesk(int desk);
    void setLuxControlReference(float value, int desk);
    int getLuxControlReference(int desk);

    // More complex getters
    float getInstantaneousPowerConsumptionAtDesk(int desk);
    std::string getElapsedTimeAtDesk(int desk);

    //Reset system
    void reset();

    // Interpret client request and return response string
    std::string processRequest(char* request);

private:
    std::chrono::time_point<std::chrono::system_clock> start;
    int numberOfDesks = 0;
    int dataAvailability = 0;
    vector< vector<float> > measuredLuxs;
     vector< vector<float> > appliedPwm;
    std::vector <int> occupancyDesk;
    std::vector <float> luxLowerBound;
    std::vector <float> luxExternal;
    std::vector <float> luxControlReference;
};

#endif //DATA_H