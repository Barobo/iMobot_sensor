#ifndef _I2C_H
#define _I2C_H

void I2cInit(void);

enum
{
    RECIEVE_ADDRESS = 2,
    CHECK_ADDRESS   = 4,
    RECIEVE_DATA    = 6,
    ACK_DATA        = 8,
    TRANSMIT_DATA   = 10,
    RECIEVE_ACK     = 12,
    CHECK_ACK       = 14
};

#endif // _I2C_H