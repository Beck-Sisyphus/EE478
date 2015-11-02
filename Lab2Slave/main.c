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
#include <adc.h>
#include <delays.h>
#include "slaveDefine.h"
#include "interrupts.h"


#pragma config FOSC = ECHP	// Oscillator
#pragma config PRICLKEN = OFF // Turn off built in clock
#pragma config WDTEN = OFF // Turn off watch dog
#pragma config CPB = OFF
//For USART
#pragma config FCMEN = OFF
#pragma config IESO = OFF
/****************Interrupt Stuff*********************/
/* Global Controls for I/O banks*/
#pragma config PBADEN = OFF   // turn off bank B ADCs

/*********Function Prototype*********/
extern void UART_init();
extern void Timer_init();
extern void ADC_init();
extern void spi_init();

extern int getTemp();
extern int bloodPres1();
extern int bloodPres2();
extern int pulseRate();

extern void display_string1(char*);


//Global Vars
char rs232out[26]  = "                ";
char rs232in[26] = "                ";
char string[] = "                ";
int i = 0;

void main (void){
    int temp;
    char readingPtr[20];
    char command[12];
    
    display_init();

    //config ADC
    ADC_init();
    Timer_init();

    clear_display("");

    command[0] = 't';


    while(1){
        
        // if (DataRdy1USART())
        // {
        //     command[0] = Read1USART();
        // }

        /********* Decode Command *********/
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

        // puts1USART(readingPtr);

        clear_display("");
        display_string1(readingPtr);
        delay(5000);

    }
}