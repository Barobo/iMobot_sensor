#include "global.h"
#include "i2c.h"
#include "hardware.h"

#define NUM_BYTES  5

char slave_data = 0x00;
char slave_address = 0x90;
int i2c_state, byte_count;

#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
    // Rx bytes from master: 
    //     GET_ADDRESS->CHECK_ADDRESS->GET_DATA->ACK_DATA
    // Tx bytes to Master:
    //     GET_ADDRESS->CHECK_ADDRESS->SEND_DATA->GET_ACK->CHECK_ACK

    if (USICTL1 & USISTTIFG)     // Start entry?
    {
        SET_HIGH(RED_LED);
        ChangeState(GET_ADDRESS);
    }
    
    switch(i2c_state)
    {           
        case GET_ADDRESS:
            USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, RX address
            USICTL1 &= ~USISTTIFG;        // Clear start flag
            ChangeState(CHECK_ADDRESS);
            break;            
        case CHECK_ADDRESS:
            if (USISRL & _BV(0))            // If master read...
            {
                slave_address |= _BV(0);             // Save R/W bit
            }
            DATA_OUTPUT();
            if (USISRL == slave_address)       // Address match?
            {
                USISRL = DATA_ACK;              // Send Ack
                SET_LOW(RED_LED);
                ChangeState(SEND_DATA);
            }
            else
            {
                USISRL = DATA_NACK;         // Send NAck
                SET_LOW(RED_LED);
                ChangeState(SEND_NACK);
            }
            USICNT |= 0x01;               // Bit counter = 1, send Ack bit
            break;
        case SEND_NACK:
            DATA_INPUT();
            slave_address = 0x90;         // Reset slave address
            ChangeState(IDLE);
            break;
        case SEND_DATA:
            DATA_OUTPUT();
            USISRL = slave_data;
            USICNT |=  0x08;              // Bit counter = 8, TX data
            ChangeState(GET_ACK);
            break;            
        case GET_ACK:
            DATA_INPUT();
            USICNT |= 0x01;               // Bit counter = 1, receive Ack
            ChangeState(CHECK_ACK);
            break;            
        case CHECK_ACK:
            if (USISRL & 0x01)               // If Nack received...
            {
                SET_HIGH(RED_LED);
            }
            else                          // Ack received
            {
                SET_LOW(RED_LED);
                USISRL = slave_data++;
            }
            DATA_INPUT();
            slave_address = 0x90;         // Reset slave address
            ChangeState(IDLE);
            break;
        default:
            break;     
    }
    USICTL1 &= ~USIIFG;                       // Clear pending flags
}

void I2cInit(void)
{
    // Config IO
    SET_HIGH(I2C_DATA);
    SET_HIGH(I2C_CLK);
    SET_RUP(I2C_DATA);
    SET_RUP(I2C_CLK);
    MAKE_OUTPUT(I2C_DATA);
    MAKE_INPUT(I2C_CLK);
    
    USICTL0 = USIPE6 + USIPE7 + USISWRST;   // Port & USI mode setup
    USICTL1 = USII2C + USIIE + USISTTIE;    // Enable I2C mode & USI interrupts
    USICKCTL = USICKPL;                     // Setup clock polarity
    USICNT |= USIIFGCC;                     // Disable automatic clear control
    USICTL0 &= ~USISWRST;                   // Enable USI
    USICTL1 &= ~USIIFG;                     // Clear pending flag
    
}

void ChangeState(uint32_t state)
{
    i2c_state = state;
}

