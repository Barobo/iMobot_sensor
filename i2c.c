#include "global.h"
#include "i2c.h"
#include "hardware.h"

#define Number_of_Bytes  5

char SLV_Data = 0x55;
char slave_address = 0x90;
int i2c_state, byte_count, transmit = 0;

#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
    // Rx bytes from master: State 2->4->6->8 
    // Tx bytes to Master: State 2->4->10->12->14

    if (USICTL1 & USISTTIFG)     // Start entry?
    {
        SET_HIGH(RED_LED);
        ChangeState(GET_ADDRESS);
    }
    
    switch(__even_in_range(i2c_state,14))
    {           
        case GET_ADDRESS:
            USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, RX address
            USICTL1 &= ~USISTTIFG;        // Clear start flag
            ChangeState(CHECK_ADDRESS);
            break;            
        case CHECK_ADDRESS:
            if (USISRL & 0x01)            // If master read...
            {
                slave_address = 0x91;             // Save R/W bit
                transmit = 1;
            }
            else
            {
                transmit = 0;
                slave_address = 0x90;
            }
            DATA_OUTPUT();
            if (USISRL == slave_address)       // Address match?
            {
                USISRL = 0x00;              // Send Ack
                SET_LOW(RED_LED);
                ChangeState(transmit ? SEND_DATA : GET_DATA);
            }
            else
            {
                USISRL = 0xFF;              // Send NAck
                SET_HIGH(RED_LED);
                ChangeState(ACK_DATA);
            }
            USICNT |= 0x01;               // Bit counter = 1, send Ack bit
            break;
            
        case GET_DATA:
            DATA_INPUT();
            USICNT |=  0x08;              // Bit counter = 8, RX data
            ChangeState(ACK_DATA);
            break;  
            
        case ACK_DATA:
            DATA_OUTPUT();
            if (byte_count <= (Number_of_Bytes-2))          // If not last byte
            {
                USISRL = 0x00;              // Send Ack
                ChangeState(GET_DATA);
                byte_count++;
                USICNT |= 0x01;             // Bit counter = 1, send Ack bit
            }
            else                          // Last Byte
            {
                USISRL = 0xFF;              // Send NAck
                DATA_INPUT();
                ChangeState(IDLE);
                byte_count =0;                 // Reset counter for next TX/RX
            }
            break;
            
        case SEND_DATA:
            DATA_OUTPUT();
            USISRL = SLV_Data++;
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
                DATA_INPUT();
                slave_address = 0x90;              // Reset slave address
                ChangeState(IDLE);
                byte_count = 0;
            }
            else                          // Ack received
            {
                SET_LOW(RED_LED);        
                DATA_OUTPUT();
                USISRL = SLV_Data++;
                USICNT |=  0x08;              // Bit counter = 8, TX data
                ChangeState(GET_ACK);
            }
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
    MAKE_OUTPUT(I2C_CLK);
    
    USICTL0 = USIPE6 + USIPE7 + USISWRST;   // Port & USI mode setup
    USICTL1 = USII2C + USIIE + USISTTIE;    // Enable I2C mode & USI interrupts
    USICKCTL = USICKPL;                     // Setup clock polarity
    USICNT |= USIIFGCC;                     // Disable automatic clear control
    USICTL0 &= ~USISWRST;                   // Enable USI
    USICTL1 &= ~USIIFG;                     // Clear pending flag
    
    transmit = 0;    
}

void ChangeState(uint32_t state)
{
    i2c_state = state;
}

