// EE 478 2015 Autumn
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <timers.h>
#include <adc.h>
#include <delays.h>
#include "slaveDefine.h"


// PORT A, 
// PIN A0 is tempIn, A1 is bp1In, A2 is bp2In, A3 is pulseIn
//     A4 is USART/RS232 Tx, A5 is USART/RS232 Rx, A7 is OscIn
// PORT B, all output for display data
// PORT C is 
// PIN C0 is RS, C1 is R/W, C2 is E, enable
// PIN C6 is UART Tx, C7 is UART Rx


void Timer_init(){
    /*
    OpenTimer0((char) TIMER_INT_OFF & T016BIT & T0SOURCE_INT & T0_PS_1_256);

    int last = 0;
    int count = 0;

    while(ReadTimer0 < 39063) //while the timer is under 1/2 second based on 20MHz clock rate
    {
        if (digitalInputPort && !last) //only count on posedge
        count = count + 1;
        last = digitalInputPort;
    }
    int BPM = count*2;
    */
    OpenTimer0( TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256);
}

void ADC_init(){
    /*
    OpenADC((char) (ADC_FOSC_RC & 
            ADC_RIGHT_JUST & 
            ADC_3ANA_0REF), 
            (char) (ADC_CH0 & ADC_INT_OFF));
    */
    TRISA = 0x0F;
    ANSELA = 0x07;
    OpenADC(ADC_FOSC_RC & 
            ADC_RIGHT_JUST & 
            ADC_12_TAD, 
            ADC_CH0 & ADC_INT_OFF, 15);
}

void spi_init(){
    //TODO:
    //OpenSPI(); //insert args
    //MISO/SDO C5
    //MISO/SDI C4
    //SCK C3
    //nSS A5
}