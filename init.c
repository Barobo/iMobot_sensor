#include "global.h"
#include "init.h"
#include "hardware.h"

void HardwareInit(void)
{
    MAKE_OUTPUT(RED_LED);
    SET_LOW(RED_LED);
    SET_HIGH(RED_LED);
    //SET_LOW(RED_LED);
}
