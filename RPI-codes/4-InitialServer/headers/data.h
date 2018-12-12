#ifndef DATA_H
#define DATA_H

class Data{
public:

    Data();

    int getAvailability();

    void setLastLuxValueArduino(int value, int arduino);
    int getLastLuxValueArduino1();
    int getLastLuxValueArduino2();

private:
    int dataAvailability = 0;
    int lastLuxValueArduino1 = 0;
    int lastLuxValueArduino2 = 0;
};

#endif //DATA_H