#include "hardware.h"
#include "timer.h"
#include "init.h"
#include "callback.h"
#include "global.h"

void BlinkLED(void);

void main(void)
{
    WD_STOP();
    
    InitTimerA();
    InitHardware();
    
    RegisterCallback(BlinkLED, 100ul * _millisecond);
    
    _EINT();
    
    while(1) { continue; }
}

void BlinkLED(void)
{
    RED_LED_TOGGLE();
}
