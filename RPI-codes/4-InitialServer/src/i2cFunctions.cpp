#include "../headers/i2cFunctions.h"

#include <iostream> //SYNC_TCP_SERVER.CPP
#include <string>
#include <stdio.h>
#include <pigpio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using namespace std;

#define SLAVE_ADDR 0x0
#define MAX_LUX 500

I2cFunctions::I2cFunctions() {
    i2cAvailability = 1;
}

int I2cFunctions::getAvailability() {
    return i2cAvailability;
}

float I2cFunctions::mapfloat(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int I2cFunctions::init_slave(bsc_xfer_t &xfer, int addr) {

    gpioSetMode(18, PI_ALT3);
    gpioSetMode(19, PI_ALT3);
    xfer.control = (addr<<16) | /* Slave address */
    (0x00<<13) | /* invert transmit status flags */
    (0x00<<12) | /* enable host control */
    (0x00<<11) | /* enable test fifo */
    (0x00<<10) | /* invert receive status flags */
    (0x01<<9) | /* enable receive */
    (0x01<<8) | /* enable transmit */
    (0x00<<7) | /* abort and clear FIFOs */
    (0x00<<6) | /* send control reg as 1st I2C byte */
    (0x00<<5) | /* send status regr as 1st I2C byte */
    (0x00<<4) | /* set SPI polarity high */
    (0x00<<3) | /* set SPI phase high */
    (0x01<<2) | /* enable I2C mode */
    (0x00<<1) | /* enable SPI mode */
    0x01 ; /* enable BSC peripheral */
    return bscXfer(&xfer);

}

int I2cFunctions::close_slave(bsc_xfer_t &xfer) {
    xfer.control = 0;
    return bscXfer(&xfer);
}

void I2cFunctions::readLoop(Data* database) {
    std::cout << "I2C thread inside readLoop database address: " << database << std::endl;
    int status, j, key = 0;
    uint16_t LuxValue = 0;
    float pwm, lux;

    if (gpioInitialise() < 0) {printf("Erro 1\n"); return;}
    
    bsc_xfer_t xfer;
    status = this->init_slave(xfer, SLAVE_ADDR);

    printf("Ready to receive data at address: 0\n");
    while(key != 'q') {

        xfer.txCnt = 0;
        status = bscXfer(&xfer);
        
        if(xfer.rxCnt > 0){
        //printf("Received %d bytes:\n", xfer.rxCnt);

            // Read I2C message type
            switch(xfer.rxBuf[1]){
                // current pwm signal
                case 0:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    pwm = this->mapfloat(xfer.rxBuf[2], 0, 255, 0, 5);
                    printf("\tPWM: %f\n\n", pwm);
                    // Add value to database
                    database->setcurrentPwmAtDesk(pwm, (int)xfer.rxBuf[0]);
                    break;
                // received lux reading
                case 1:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];
                    lux = this->mapfloat(LuxValue, 0, 65536, 0, MAX_LUX);  
                    printf("\tLux: %f\n\n", lux);
                    // Add value to database
                    database->setLastLuxValueArduino(lux, (int)xfer.rxBuf[0]);
                    break;
                // received pwm negotiation message
                /*case 2:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    pwm = this->mapfloat(xfer.rxBuf[2], 0, 5, 0, 255);
                    printf("Negotiation: %f\n\n", pwm);
                    break;*/
                // occupancy
                case 3:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    printf("\tOccupancy: %d\n\n", xfer.rxBuf[2]);
                    database->setOccupancyAtDesk(xfer.rxBuf[2], xfer.rxBuf[0]);
                    break;
                // lux lower bound
                case 4:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];
                    lux = this->mapfloat(LuxValue, 0, 65536, 0, MAX_LUX);
                    printf("\tLux Lower Bound: %f\n\n", lux);
                    // Add value to database
                    database->setLuxLowerBoundAtDesk(lux, xfer.rxBuf[0]);
                    break;
                // background lux
                case 5:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];
                    lux = this->mapfloat(LuxValue, 0, 65536, 0, MAX_LUX);
                    printf("\tBackground Lux: %f\n\n", lux);
                    // Add value to database
                    database->setLuxExternalAtDesk(lux, xfer.rxBuf[0]);
                    break;
                // control reference pwm
                case 6:
                    printf("Arduino %d ", xfer.rxBuf[0]);
                    pwm = this->mapfloat(xfer.rxBuf[2], 0, 255, 0, 5);
                    printf("\tReference pwm: %f\n\n", pwm);
                    // Add value to database
                    database->setLuxControlReference(pwm, xfer.rxBuf[0]);
                    break;
            }

        }
        
        //printf("\n Press q to quit.\n");
        //key = getchar();
    }
    status = this->close_slave(xfer);
    gpioTerminate();
}