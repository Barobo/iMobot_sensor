#include "global.h"
#include "i2c.h"
#include "hardware.h"
#include "endpoints.h"

uint8_t current_address = 0x02;
uint8_t device_type = 0x00;
uint8_t state = IDLE;
uint8_t read = 0;
uint8_t reg = 0;

#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
    if (START_ISSUED)
    {
        ChangeState(HANDLE_START_BIT);
    }
    
    switch(state)
    {
        case HANDLE_START_BIT:
            ChangeState(HANDLE_ADDRESS_AND_READ_WRITE);
            DATA_INPUT();
            set_bits_remaining(8);
            CLEAR_START();
            break;
            
        case HANDLE_ADDRESS_AND_READ_WRITE:
            if ((DATA_BUFFER & ADDRESS_MASK) == current_address)
            {
                read = DATA_BUFFER & READ_BIT;
                ChangeState(HANDLE_ADDRESS_ACK);
                DATA_OUTPUT();
                DATA_BUFFER = DATA_ACK;
                set_bits_remaining(1);
            }
            else // address did not match
            {
                DATA_INPUT();
                // TODO: should we just ignore bad addresses?
                //DATA_BUFFER = DATA_NACK;         // Send NAck
                ChangeState(IDLE);
            }
            break;
            
        case HANDLE_ADDRESS_ACK:
            if (read)
            {
                ChangeState(HANDLE_DATA_READ);
                DATA_OUTPUT();
                if (reg == I2C_ADDR_ADDRESS)
                {
                    DATA_BUFFER = current_address;
                }
                else if (reg == I2C_ADDR_TYPE)
                {
                    DATA_BUFFER = device_type;
                }
                else if (reg == I2C_ADDR_NUM_READ_ENDPOINTS)
                {
                    DATA_BUFFER = GetNumReadEndpoints();
                }
                else if (reg == I2C_ADDR_START_READ_ENDPOINTS)
                {
                    DATA_BUFFER = READ_ENDPOINTS_START;
                }
                else if (reg == I2C_ADDR_NUM_WRITE_ENDPOINTS)
                {
                    DATA_BUFFER = GetNumWriteEndpoints();
                }
                else if (reg == I2C_ADDR_START_WRITE_ENDPOINTS)
                {
                    DATA_BUFFER = WRITE_ENDPOINTS_START;
                }
                else if (reg >= READ_ENDPOINTS_START && reg < READ_ENDPOINTS_START + GetNumReadEndpoints())
                {
                    DATA_BUFFER = GetReadEndpoint(reg - READ_ENDPOINTS_START);
                }
                else
                {
                    // todo: this should nack as well I think - KSH
                    DATA_BUFFER = 0x00;
                }
                set_bits_remaining(8);                
            }
            else
            {
                ChangeState(HANDLE_REGISTER);
                DATA_INPUT();
                set_bits_remaining(8);
            }
            break;

        case HANDLE_REGISTER:
            reg = DATA_BUFFER;
            ChangeState(HANDLE_REGISTER_ACK);
            DATA_OUTPUT();
            DATA_BUFFER = DATA_ACK;
            set_bits_remaining(1);
            break;

        case HANDLE_REGISTER_ACK:
            ChangeState(HANDLE_DATA_WRITTEN);
            DATA_INPUT();
            set_bits_remaining(8);
            break;
        
        case HANDLE_DATA_READ:
            ChangeState(HANDLE_DATA_ACK);
            DATA_OUTPUT();
            DATA_BUFFER = DATA_ACK;
            set_bits_remaining(1);
            break;
            
        case HANDLE_DATA_WRITTEN:
            ChangeState(HANDLE_DATA_ACK);
            DATA_OUTPUT();
            if (reg == I2C_ADDR_ADDRESS)
            {
                // todo: find a way to store this in NVRAM
                current_address = DATA_BUFFER;
            }
            else if (reg >= WRITE_ENDPOINTS_START && reg < WRITE_ENDPOINTS_START + GetNumWriteEndpoints())
            {
                SetWriteEndpoint(reg - WRITE_ENDPOINTS_START, DATA_BUFFER);
            }
            else
            {
                DATA_BUFFER = DATA_NACK;
                set_bits_remaining(1);
                break;
            }
            DATA_BUFFER = DATA_ACK;
            set_bits_remaining(1);
            break;

        case HANDLE_DATA_ACK:
            DATA_INPUT();
            ChangeState(IDLE);
            break;
   
        default:
            break;
    }
    CLEAR_INT();
}

void I2cInit(uint8_t addr, uint8_t type)
{
    current_address = addr;
    device_type = type;
    
    // Config IO
    SET_HIGH(I2C_DATA);
    SET_HIGH(I2C_CLK);
    SET_RUP(I2C_DATA);
    SET_RUP(I2C_CLK);
    MAKE_OUTPUT(I2C_DATA);
    //MAKE_INPUT(I2C_CLK);
    MAKE_OUTPUT(I2C_CLK);
    
    USICTL0 = USIPE6 + USIPE7 + USISWRST;   // Port & USI mode setup
    USICTL1 = USII2C + USIIE + USISTTIE;    // Enable I2C mode & USI interrupts
    USICKCTL = USICKPL;                     // Setup clock polarity
    USICNT |= USIIFGCC;                     // Disable automatic clear control
    USICTL0 &= ~USISWRST;                   // Enable USI
    USICTL1 &= ~USIIFG;                     // Clear pending flag    
}

void ChangeState(uint8_t in_state)
{
    state = in_state;
}

void set_bits_remaining(uint8_t cnt)
{
    USICNT = (USICNT & (~BIT_COUNT_MASK)) | (cnt & BIT_COUNT_MASK);
}