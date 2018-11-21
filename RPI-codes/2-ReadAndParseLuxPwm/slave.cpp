#include <stdio.h>
#include <pigpio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define SLAVE_ADDR 0x0

int init_slave(bsc_xfer_t &xfer, int addr) {
    
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

int close_slave(bsc_xfer_t &xfer) {

    xfer.control = 0;
    return bscXfer(&xfer);
}

int main(int argc, char *argv[]) {
    int status, j, key = 0;
    int luxValue = 0;
    int pwmValue = 0;

    if (gpioInitialise() < 0) {printf("Erro 1\n"); return 1;}
    
    bsc_xfer_t xfer;
    status = init_slave(xfer, SLAVE_ADDR);

    cout << "Ready to receive data at address: 0\n" << endl;
    while(key != 'q') {

        xfer.txCnt = 0;
        status = bscXfer(&xfer);
        
        if(xfer.rxCnt > 0){
        printf("Received %d bytes:\n", xfer.rxCnt);

        switch(xfer.rxBuf[1]){
            // Initial lux
            case 0:
                luxValue = xfer.rxBuf[2] + xfer.rxBuf[3];
                cout << "Arduino " << xfer.rxBuf[0] << endl;
                cout << "Initial lux value: " << luxValue << endl;
                printf("\n");
                break;
            // Periodic lux
            case 1:
                luxValue = xfer.rxBuf[2] + xfer.rxBuf[3];
                cout << "Arduino " << xfer.rxBuf[0] << endl;
                cout << "Current lux value: " << luxValue << endl;
                printf("\n");
                break;
            // Initial pwm
            case 2:
                cout << "Arduino " << xfer.rxBuf[0] << endl;
                cout << "Initial pwm value: " << xfer.rxBuf[2] << endl;
                break;
            // Periodic pwm
            case 3: 
                cout << "Arduino " << xfer.rxBuf[0] << endl;
                cout << "Current pwm value: " << xfer.rxBuf[2] << endl;
                break
            default:
                cout << "Invalid message received:" << endl;
                for(j=0;j<xfer.rxCnt;j++)
                    printf("%c\n",xfer.rxBuf[j]);
                printf("\n");
        }

        }

        for(j=0;j<xfer.rxCnt;j++)
            printf("%c\n\n",xfer.rxBuf[j]);
        
        //printf("\n Press q to quit.\n");
        //key = getchar();
    }
    status = close_slave(xfer);
    gpioTerminate();
}

