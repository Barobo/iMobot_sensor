#ifndef _I2C_H
#define _I2C_H

void I2cInit(void);
void ChangeState(uint32_t state);

enum
{
    IDLE            = 0,
    GET_ADDRESS     = 1,   
    CHECK_ADDRESS   = 2,
    SEND_NACK       = 3,
    SEND_DATA       = 4,
    GET_ACK         = 5,
    CHECK_ACK       = 6,
};

#define DATA_OUTPUT() {USICTL0 |= USIOE;}
#define DATA_INPUT()  {USICTL0 &= ~USIOE;}

#define DATA_ACK  0
#define DATA_NACK 0xFF




#endif // _I2C_H