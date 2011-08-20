#ifndef HARDWARE_H
#define HARDWARE_H

//macros
#define st(x) do{x} while(__LINE__ == -1)

#define MAKE_OUTPUT(pin) IO_MAKE_OUTPUT(1,pin)
#define MAKE_INPUT(pin)  IO_MAKE_INPUT(1, pin)
#define SET_HIGH(pin)    IO_SET_HIGH(1,pin)
#define SET_LOW(pin)     IO_SET_LOW(1,pin)
#define SET_RUP(pin)     IO_SET_PULLUP(1,pin)
#define SET_RDOWN(pin)   IO_SET_PULLDOWN(1,pin)
#define TOGGLE(pin)      IO_TOG(1,pin)
#define READ_IN(pin)     IO_DIG_READ(1,pin)

#define IO_MAKE_OUTPUT(port,pin)  st((P##port##DIR |=  _BV(pin));)
#define IO_MAKE_INPUT(port, pin)  st((P##port##DIR &= ~_BV(pin));)
#define IO_SET_HIGH(port,pin)     st((P##port##OUT |=  _BV(pin));)
#define IO_SET_LOW(port,pin)      st((P##port##OUT &= ~_BV(pin));)
#define IO_SET_PULLUP(port,pin)   st((P##port##REN |=  _BV(pin));)
#define IO_SET_PULLDOWN(port,pin) st((P##port##REN &= ~_BV(pin));)
#define IO_TOG(port,pin)          st((P##port##OUT ^= _BV(pin));)
#define IO_DIG_READ(port,pin)     !(P##port##IN    &  _BV(pin))

#ifdef LAUNCHPAD
#define RED_LED   0
#define LP_SWITCH 3
#endif

#define I2C_DATA 7
#define I2C_CLK  6
#define A0       0
#define A1       1
#define A2       2
#define A3       3
#define A4       4
#define A5       5



#endif
