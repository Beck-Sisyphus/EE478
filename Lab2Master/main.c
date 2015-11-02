/*
 * File:   main.c
 * Author: pshfls
 *
 * Created on Nov 1, 2015, 10:30 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <timers.h>
#include <usart.h>
#include <delays.h>
#include "masterDefine.h"

//#include <adc.h>
#include <timers.h>

#pragma config FOSC = INTIO7 // Oscillator, ECHP for 20MHz external oscillator, INTIO7 for 16MHz internal oscillator
#pragma config PRICLKEN = ON // Turn off built in clock
#pragma config WDTEN = OFF // Turn off watch dog
#pragma config CPB = OFF
//For USART
#pragma config FCMEN = OFF
#pragma config IESO = OFF


/*********Function Prototype*********/
extern void UART_init();
extern void UART_write(const char* commandPtr, char* readingPtr);
extern void UART_read(char* commandPtr);
extern void SPI_init();


void main (void){
    char commandPtr[20];
    char readingPtr[20];
    char temp[10] = "test";

//    TRISA = 0x0F;
//    ANSELA = 0x07;
//    OpenADC(ADC_FOSC_RC &
//            ADC_RIGHT_JUST &
//            ADC_12_TAD,
//            ADC_CH0 & ADC_INT_OFF, 15);
    OpenTimer0( TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);

    UART_init();
    SPI_init();

    commandPtr[0] = 't';

    while(1){
//        if (DataRdy1USART())
//        {
//            UART_read(command);
//        }
//        UART_write(command, reading);

        if (DataRdy1USART())
        {
            commandPtr[0] = Read1USART();
        }

        sprintf(readingPtr,"Pulse: %02u \n", temp[0]);
     
        puts1USART(readingPtr);

        delay(5000);
    }
}