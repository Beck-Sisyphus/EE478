/* 
 * File:   commUART.c
 * Author: Beck
 *
 * Created on November 1, 2015, 3:35 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <usart.h>
#include <delays.h>

#define BAUD_9600_20MHz 129

void UART_init(){
    // PIN A0 is  temp[0]In, A1 is bp1In, A2 is bp2In, A3 is pulseIn
    
    //C6 is USART/RS232 Tx, C7 is USART/RS232 Rx
    
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    
    // Enable digital for all c pins
    ANSELC=0x00;
    //LATC  =0x00;
    //TRISC =0x00;
    
    RCONbits.IPEN = 1; // turn on priorities
    IPR1bits.RCIP = 1; // make Rx interrupt high priority
    INTCONbits.GIEH = 1; // turn on high priority interrupts
    
    Open1USART(USART_TX_INT_OFF  &
            USART_RX_INT_OFF   &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT   &
            //USART_CONT_RX     &
            USART_BRGH_HIGH, 
            BAUD_9600_20MHz);   //Set Baud Rate here  Fosc / (64 * (spbrg + 1))

    Write1USART(0x0c);   // clear hyperterminal
}

void UART_write(char* commandPtr, char* readingPtr)
{
    char temp[10] = "test";
    
    if (commandPtr[0] == 'p') {
        // read the pulse
        temp[0] = pulseRate();
        sprintf(readingPtr,"Pulse: %02u \n", temp[0]);
        delay(1000);
    }
    else if (commandPtr[0] == 's') {
        // read the systolic
        temp[0] = bloodPres1();
        sprintf(readingPtr,"Syst: %02u \n", temp[0]);
        delay(1000);
    }
    else if (commandPtr[0] == 'd') {
        // read the diastolic
        temp[0] = bloodPres2();
        sprintf(readingPtr,"Diast: %02u \n", temp[0]);
        delay(1000);
    }
    else if (commandPtr[0] == 't') {
        // read the temp[0]
        temp[0] = getTemp[0]();
        sprintf(readingPtr,"Temp[0]: %02u \n", temp[0]);
        delay(1000);
    }
    puts1USART(readingPtr);
}

void UART_read(char* commandPtr)
{
    commandPtr[0] = Read1USART();
}

// Timer
void delay(int count) 
{
  int i, j;
  for (i = 0; i < count; ++i)
  {
    for (j = 0; j < 50; ++j)
    {
    }
  }
} 