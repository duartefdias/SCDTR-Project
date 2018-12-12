#ifndef I2CFUNCTIONS_H
#define I2CFUNCTIONS_H

class I2cFunctions{
public:

    I2cFunctions();
    int getAvailability();

    static void readLoop();

private:

    int i2cAvailability = 0;

};

#endif //I2CFUNCTIONS_H