#ifndef GLOBAL_H
#define GLOBAL_H

#include "msp430g2231.h"

// sensor types
enum
{
    IR_DIGITAL,
    IR_ANALOG,
    ACCELERATION_1D,
    ACCELERATION_2D,
    ACCELERATION_3D,
    GYRO_1D,
    GYRO_2D,
    GYRO_3D,
    FLASHLIGHT,
    COMPASS,
    MOISTURE,
    GPS,
    TEMPERATURE,
    GRIPPER
};

#define SENSOR_TYPE IR_SENSOR_DIGITAL

#define SLAVE_ADDRESS 0x90

#define LAUNCHPAD

#define CLOCK_DCO   1000000                                 // DCO freq
#define CLOCK_EXT   32768                                   // ext clock freq

#define INT_CNT_DCO	((int)(0.0005 * CLOCK_DCO))
#define INT_CNT_EXT	((int)(0.01 * CLOCK_EXT))

#define TRUE 1
#define FALSE 0

// macros
#define _BV(bit) (1<<bit)

#define WD_STOP() (WDTCTL = WDTPW + WDTHOLD)

#define CAL_CLOCK()\
{\
    BCSCTL1 = CALBC1_1MHZ;\
    DCOCTL = CALDCO_1MHZ;\
}

//types
typedef unsigned char      uint8_t;
typedef signed char        int8_t;
typedef unsigned short int uint16_t;
typedef signed short int   int16_t;
typedef unsigned long int  uint32_t;
typedef signed long int    int32_t;

#endif
