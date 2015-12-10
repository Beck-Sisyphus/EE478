#include "driverlib.h"
#include "project.h"
//#include "display.c"

#include <stdint.h>
#include <stdbool.h>

//global vars, put into main
extern bool oldSwitch1;
extern bool newSwitch1;
extern bool oldSwitch2;
extern bool newSwitch2;
extern bool oldSwitch3;
extern bool newSwitch3;
extern bool oldSwitch4;
extern bool newSwitch4;
extern bool oldSwitch5;
extern bool newSwitch5;

/*
//Prototypes
void switch_init();
void updateSwitches();
*/

//init out pins, put into main (or somewhere else)
void switch_init(){
	//switch1on pin 10.0
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN0);
	//MAP_GPIO_setOutputHighOnPin (GPIO_PORT_P10, GPIO_PIN0);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN0);
	//MAP_GPIO_setDriveStrengthHigh (GPIO_PORT_P10, GPIO_PIN0);

	//switch1off pin 10.1
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN1);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN1);
	//MAP_GPIO_setDriveStrengthHigh (GPIO_PORT_P10, GPIO_PIN1);

	//switch2on pin 10.2
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN2);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN2);

	//switch2off pin 10.3
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN3);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN3);

	//switch3on pin 10.4
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN4);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN4);

	//switch3off pin 10.5
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, GPIO_PIN5);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P10, GPIO_PIN5);

	//switch4on pin 7.4
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P7, GPIO_PIN4);

	//switch4off pin 7.5
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P7, GPIO_PIN5);

	//switch5on pin 7.6
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN6);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P7, GPIO_PIN6);

	//switch5off pin 7.7
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN7);
	MAP_GPIO_setOutputLowOnPin (GPIO_PORT_P7, GPIO_PIN7);
}

//update function
void updateSwitches(){
	int ii;

	if (oldSwitch1!=newSwitch1){
		if(newSwitch1){
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN0);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch1 On!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN0);
		}
		else{
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN1);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch1 Off!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN1);
		}
		oldSwitch1 = newSwitch1;
	}

	if (oldSwitch2!=newSwitch2){
		if(newSwitch2){
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN2);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch2 On!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN2);
		}
		else{
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN3);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch2 Off!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN3);
		}
		oldSwitch2 = newSwitch2;
	}

	if (oldSwitch3!=newSwitch3){
		if(newSwitch3){
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN4);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch3 On!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN4);
		}
		else{
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN5);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch3 Off!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P10, GPIO_PIN5);
		}
		oldSwitch3 = newSwitch3;
	}

	if (oldSwitch4!=newSwitch4){
		if(newSwitch4){
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch4 On!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
		}
		else{
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch4 Off!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);
		}
		oldSwitch4 = newSwitch4;
	}

	if (oldSwitch5!=newSwitch5){
		if(newSwitch5){
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN6);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch5 On!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN6);
		}
		else{
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN7);
        	for(ii=0;ii<200000;ii++);
        	clear_display();
    		displayString("Switch5 Off!");
        	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN7);
		}
		oldSwitch5 = newSwitch5;
	}
}
