#include "global.h"
#include "hardware.h"
#include "timer.h"
#include "init.h"
#include "callback.h"
#include "i2c.h"

void main(void)
{
    WD_STOP();
    CAL_CLOCK();
    
//    TimerAInit();
    HardwareInit();
    I2cInit();
    _EINT();
    
    LPM0;       // CPU off, await USI interrupt
    _NOP();
}

void BlinkLED(void)
{
    TOGGLE(RED_LED);
}
