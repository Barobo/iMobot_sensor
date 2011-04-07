#ifndef _I2C_H
#define _I2C_H

void I2cInit(void);
void ChangeState(uint32_t state);

enum
{
    IDLE            = 0,
    GET_ADDRESS     = 2,
    CHECK_ADDRESS   = 4,
    GET_DATA        = 6,
    ACK_DATA        = 8,
    SEND_DATA       = 10,
    GET_ACK         = 12,
    CHECK_ACK       = 14
};

#define DATA_OUTPUT() {USICTL0 |= USIOE;}
#define DATA_INPUT()  {USICTL0 &= ~USIOE;}



#endif // _I2C_H