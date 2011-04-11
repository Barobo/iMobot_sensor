#ifndef _I2C_H
#define _I2C_H

void I2cInit(void);
void ChangeState(uint32_t state);

enum
{
    GET_ADDRESS    ,
    CHECK_ADDRESS  ,
    GET_DATA       ,
    ACK_DATA       ,
    SEND_DATA      ,
    GET_ACK        ,
    CHECK_ACK
};

#define DATA_OUTPUT() {USICTL0 |= USIOE;}
#define DATA_INPUT()  {USICTL0 &= ~USIOE;}

#define DATA_ACK  0
#define DATA_NACK 0xFF




#endif // _I2C_H