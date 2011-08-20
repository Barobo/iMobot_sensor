#ifndef _I2C_H
#define _I2C_H

void I2cInit(uint8_t addr, uint8_t type);
void ChangeState(uint8_t in_state);
void set_bits_remaining(uint8_t cnt);
enum
{
    IDLE,
    HANDLE_START_BIT,
    HANDLE_ADDRESS_AND_READ_WRITE,
    HANDLE_ADDRESS_ACK,
    HANDLE_REGISTER,
    HANDLE_REGISTER_ACK,
    HANDLE_DATA_READ,
    HANDLE_DATA_WRITTEN,
    HANDLE_DATA_ACK,
};

enum
{
    I2C_ADDR_ADDRESS = 0x01,
    I2C_ADDR_TYPE = 0x02,
    I2C_ADDR_NUM_READ_ENDPOINTS = 0x03,
    I2C_ADDR_START_READ_ENDPOINTS = 0x04,
    I2C_ADDR_NUM_WRITE_ENDPOINTS = 0x05,
    I2C_ADDR_START_WRITE_ENDPOINTS = 0x06,
    READ_ENDPOINTS_START = 0x10,
    WRITE_ENDPOINTS_START = 0x90,
};

#define DATA_OUTPUT() {USICTL0 |= USIOE;}
#define DATA_INPUT()  {USICTL0 &= ~USIOE;}
#define CLEAR_START() {USICTL1 &= ~USISTTIFG;}
#define CLEAR_INT()   {USICTL1 &= ~USIIFG;}
#define START_ISSUED  (USICTL1 & USISTTIFG)

#define DATA_BUFFER    USISRL
#define DATA_ACK       0x00
#define DATA_NACK      0xFF
#define ADDRESS_MASK   0xFE
#define READ_BIT       0x01
#define BIT_COUNT_MASK 0x1F

#define NACK_RESPONSE  0x01

#endif // _I2C_H