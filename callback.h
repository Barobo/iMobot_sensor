#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "global.h"

typedef void (*SchedulerCallback)(void);

#define MAX_CALLBACK_CNT 2

enum
{
    DISABLED = 0,
    ENABLED = 1
};

typedef struct
{
    SchedulerCallback func;
    uint8_t enabled;
    uint32_t run_time;
    uint32_t next_run_time;
} ScheduledEvent;

// Prototypes
int32_t CallbackRegister(SchedulerCallback callbackFunction, uint32_t run_time);
void    CallbackService(uint32_t current_time);
int32_t CallbackEnable(SchedulerCallback func);
int32_t CallbackDisable(SchedulerCallback func);

// Base timing for callbacks (minimum 100uS)
#define _100micro  1
#define _10milli   100
#define _100milli  1000
#define _second    10000
#define _minute    600000
#define _hour      36000000

#endif
