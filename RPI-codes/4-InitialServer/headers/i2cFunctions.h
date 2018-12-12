#ifndef I2CFUNCTIONS_H
#define I2CFUNCTIONS_H

#include <stdio.h>
#include <pigpio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

class I2cFunctions{
public:

    I2cFunctions();
    int getAvailability();

    static void readLoop();

private:

    float mapfloat(double val, double in_min, double in_max, double out_min, double out_max);
    int init_slave(bsc_xfer_t &xfer, int addr);
    int close_slave(bsc_xfer_t &xfer);

    int i2cAvailability = 0;

};

#endif //I2CFUNCTIONS_H