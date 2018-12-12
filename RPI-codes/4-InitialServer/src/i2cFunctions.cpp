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

void I2cFunctions::readLoop(Data database) {
    int status, j, key = 0;
    uint16_t LuxValue = 0;
    float pwm;

    if (gpioInitialise() < 0) {printf("Erro 1\n"); return;}
    
    bsc_xfer_t xfer;
    status = this->init_slave(xfer, SLAVE_ADDR);

    printf("Ready to receive data at address: 0\n");
    while(key != 'q') {

        xfer.txCnt = 0;
        status = bscXfer(&xfer);
        
        if(xfer.rxCnt > 0){
        //printf("Received %d bytes:\n", xfer.rxCnt);

            switch(xfer.rxBuf[1]){
                //pwm
                case 0:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    pwm = this->mapfloat(xfer.rxBuf[2], 0, 5, 0, 255);
                    printf("PWM: %d\n\n", pwm);
                    printf("\n");
                    break;
                //lux
                case 1:
                    //printf("Arduino %d\n", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];
                    LuxValue = mapfloat(LuxValue, 0, 65536, 0, MAX_LUX);  
                    printf("Lux: %d\n\n", LuxValue);
                    // Add value to database
                    database.setLastLuxValueArduino(LuxValue, xfer.rxBuf[0]);
                    printf("\n");
                    break;
                //pwm negotiation
                case 2:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    pwm = this->mapfloat(xfer.rxBuf[2], 0, 5, 0, 255);
                    printf("Negotiation: %d\n\n", pwm);
                    break;
                //occupancy
                case 3:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    printf("Occupancy: %d\n\n", xfer.rxBuf[2]);
                    break;
                //lux lower bound
                case 4:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];  
                    printf("Lux Lower Bound: %d\n\n", LuxValue);
                    printf("\n");
                    break;
                //background lux
                case 5:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];  
                    printf("Background Lux: %d\n\n", LuxValue);
                    printf("\n");
                    break;
                //reference lux
                case 6:
                    printf("Arduino %d\n", xfer.rxBuf[0]);
                    LuxValue = xfer.rxBuf[2];
                    LuxValue <<= 8;
                    LuxValue |= xfer.rxBuf[3];  
                    printf("Reference Lux: %d\n\n", LuxValue);
                    printf("\n");
                    break;
            }

        }
        
        //printf("\n Press q to quit.\n");
        //key = getchar();
    }
    status = this->close_slave(xfer);
    gpioTerminate();
}