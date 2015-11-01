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
#include <adc.h>
#include <delays.h>
#include "p18f25k22Define.h"
#include "interrupts.h"

//Global Vars
char rs232out[6]  = "                ";
char rs232in[6] = "                ";
char string[] = "                ";
int i = 0;

void main (void){
    int temp;
    char readingPtr[20];
    char command[12];
    
    display_init();
    UART_init();
    Write1USART(0x0c);   // clear hyperterminal

    //config ADC
    TRISA = 0x0F;
    ANSELA = 0x07;
    OpenADC(ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH0 & ADC_INT_OFF, 15);
    OpenTimer0( TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
    clear_display("");

    command[0] = 't';


    while(1){

        if (DataRdy1USART())
        {
            command[0] = Read1USART();
        }

        if (command[0] == 'p') {
            // read the pulse
            temp = pulseRate();
            sprintf(readingPtr,"Pulse: %02u \n", temp);
            delay(1000);
        }
        else if (command[0] == 's') {
            // read the systolic
            temp = bloodPres1();
            sprintf(readingPtr,"Syst: %02u \n", temp);
            delay(1000);
        }
        else if (command[0] == 'd') {
            // read the diastolic
            temp = bloodPres2();
            sprintf(readingPtr,"Diast: %02u \n", temp);
            delay(1000);
        }
        else if (command[0] == 't') {
            // read the temp
            temp = getTemp();
            sprintf(readingPtr,"Temp: %02u \n", temp);
            delay(1000);
        }

        puts1USART(readingPtr);
        clear_display("");
        display_string1(readingPtr);
        delay(5000);

    }
}