#include "global.h"
#include "hardware.h"
#include "timer.h"
#include "init.h"
#include "callback.h"
#include "i2c.h"
#include "endpoints.h"

void main(void)
{
    WD_STOP();
    CAL_CLOCK();
    
//    TimerAInit();
    HardwareInit();
    I2cInit(0x02, 0x00);
    _EINT();
    
    while (1)
    {
        //LPM0;
        //_NOP();
        if (GetWriteEndpoint(0))
            SET_LOW(RED_LED);
        else
            SET_HIGH(RED_LED);
        
        SetReadEndpoint(0, READ_IN(LP_SWITCH));
    }
}
