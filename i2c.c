#include "global.h"
#include "i2c.h"
#include "hardware.h"

#define Number_of_Bytes  5

char MST_Data = 0;
char SLV_Data = 0x55;
char SLV_Addr = 0x90;
int I2C_State, Bytecount, transmit = 0;

#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
    // Rx bytes from master: State 2->4->6->8 
    // Tx bytes to Master: State 2->4->10->12->14

    if (USICTL1 & USISTTIFG)     // Start entry?
    {
        P1OUT |= 0x01;                          // LED on: sequence start
        I2C_State = 2;                          // Enter 1st state on start
    }
    
    switch(__even_in_range(I2C_State,14))
    {           
        case RECIEVE_ADDRESS:
            USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, RX address
            USICTL1 &= ~USISTTIFG;        // Clear start flag
            I2C_State = 4;                // Go to next state: check address
            break;            
        case CHECK_ADDRESS:
            if (USISRL & 0x01)            // If master read...
            {
                SLV_Addr = 0x91;             // Save R/W bit
                transmit = 1;
            }
            else
            {
                transmit = 0;
                SLV_Addr = 0x90;
            }
            USICTL0 |= USIOE;             // SDA = output
            if (USISRL == SLV_Addr)       // Address match?
            {
                USISRL = 0x00;              // Send Ack
                P1OUT &= ~0x01;             // LED off
                if (transmit == 0)
                { 
                    I2C_State = 6;           // Go to next state: RX data
                }
                if (transmit == 1)
                {  
                    I2C_State = 10;          // Else go to next state: TX data
                }
            }
            else
            {
                USISRL = 0xFF;              // Send NAck
                P1OUT |= 0x01;              // LED on: error
                I2C_State = 8;              // next state: prep for next Start
            }
            USICNT |= 0x01;               // Bit counter = 1, send Ack bit
            break;
            
        case RECIEVE_DATA:
            USICTL0 &= ~USIOE;            // SDA = input
            USICNT |=  0x08;              // Bit counter = 8, RX data
            I2C_State = 8;                // next state: Test data and (N)Ack
            break;  
            
        case ACK_DATA:
            USICTL0 |= USIOE;             // SDA = output
            if (Bytecount <= (Number_of_Bytes-2))          // If not last byte
            {
                USISRL = 0x00;              // Send Ack
                I2C_State = 6;              // Rcv another byte
                Bytecount++;
                USICNT |= 0x01;             // Bit counter = 1, send Ack bit
            }
            else                          // Last Byte
            {
                USISRL = 0xFF;              // Send NAck
                USICTL0 &= ~USIOE;            // SDA = input
                SLV_Addr = 0x90;              // Reset slave address
                I2C_State = 0;                // Reset state machine
                Bytecount =0;                 // Reset counter for next TX/RX
            }
            break;
            
        case TRANSMIT_DATA:
            USICTL0 |= USIOE;             // SDA = output
            USISRL = SLV_Data++;
            USICNT |=  0x08;              // Bit counter = 8, TX data
            I2C_State = 12;               // Go to next state: receive (N)Ack
            break;
            
        case RECIEVE_ACK:
            USICTL0 &= ~USIOE;            // SDA = input
            USICNT |= 0x01;               // Bit counter = 1, receive Ack
            I2C_State = 14;               // Go to next state: check Ack
            break;
            
        case CHECK_ACK:
            if (USISRL & 0x01)               // If Nack received...
            {
                USICTL0 &= ~USIOE;            // SDA = input
                SLV_Addr = 0x90;              // Reset slave address
                I2C_State = 0;                // Reset state machine
                Bytecount = 0;
            }
            else                          // Ack received
            {
                P1OUT &= ~0x01;             // LED off            
                USICTL0 |= USIOE;             // SDA = output
                USISRL = SLV_Data++;
                USICNT |=  0x08;              // Bit counter = 8, TX data
                I2C_State = 12;               // Go to next state: receive (N)Ack
            }
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


