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

// #pragma config FOSC = ECHP	// Oscillator
// #pragma config PRICLKEN = OFF // Turn off built in clock
// #pragma config WDTEN = OFF // Turn off watch dog
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
    char command[20];
    char reading[20];

    UART_init();
    SPI_init();

    command[0] = 't';

    while(1){
        if (DataRdy1USART())
        {
            UART_read(command);
        }
        UART_write(command, reading);


        delay(5000);
    }
}