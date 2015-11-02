#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <adc.h>
#include <timers.h>
#include "slaveDefine.h"

int getTemp(){
    int Tempresult;
    int Temperature;
    SetChanADC(ADC_CH2); //Temperature
    ConvertADC();
    while(BusyADC());
    Tempresult = ReadADC();
    Temperature = (Tempresult-35)/4.5 +55;
    return Temperature;
}

int bloodPres1(){
    int BP1;
    int BP1result;
    SetChanADC(ADC_CH0); //Blood Pressure 1
    ConvertADC();
    while(BusyADC());
    BP1result = ReadADC();
    BP1 = BP1result*3/4;
    return BP1;
}

int bloodPres2() {
    int BP2;
    int BP2result;
    SetChanADC(ADC_CH1); //Blood Pressure 2
    ConvertADC();
    while(BusyADC());
    BP2result = ReadADC();
    BP2 = BP2result*3/4;
    return BP2;
}

int pulseRate(){
    int BPM;
    int last = 0;
    int count = 0;
    
    WriteTimer0(0);
    while(ReadTimer0() < 62500) //while the timer is under 1/2 second based on 20MHz clock rate
    {
        if (PORTAbits.RA3 && !last)//only count on posedge
        {
            count = count + 1;
        }
        last = PORTAbits.RA3;
    }
    
    BPM = (count/3);
    return BPM;
}
