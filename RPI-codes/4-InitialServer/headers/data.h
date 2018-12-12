#ifndef DATA_H
#define DATA_H

class Data{
public:

    Data();

    int getAvailability();

    void setLastLuxValueArduino1(int value);
    int getLastLuxValueArduino1();

    void setLastLuxValueArduino2(int value);
    int getLastLuxValueArduino2();

private:
    int dataAvailability = 0;
    int lastLuxValueArduino1 = 0;
    int lastLuxValueArduino2 = 0;
};

#endif //DATA_H