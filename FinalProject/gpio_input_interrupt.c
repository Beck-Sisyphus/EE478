#include "driverlib.h"
#include "project.h"
//#include "display.c"
//#include "switch.c"

#include <stdint.h>
#include <stdbool.h>

//Prototypes
//Switch Stuff
extern void switch_init();
extern void updateSwitches();

//Display Stuff
extern void display_init();
extern void clear_display();
extern void displayString(char*);

//Global Variables
bool oldSwitch1 = false;
bool newSwitch1 = false;
bool oldSwitch2 = false;
bool newSwitch2 = false;
bool oldSwitch3 = false;
bool newSwitch3 = false;
bool oldSwitch4 = false;
bool newSwitch4 = false;
bool oldSwitch5 = false;
bool newSwitch5 = false;

bool doorSensor1 = false;
bool doorSensor2 = false;
bool windowSensor1 = false;
bool windowSensor2 = false;

int main(void){
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
    
    /*
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5); //Set P2.2/Blue LED to digital out
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1);
    SysTick_enableModule();
    */
    
    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    /*Initialize Sensor Ports  7.0-7.3   In Progress!!!
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P7, GPIO_PIN0);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P7, GPIO_PIN0;
    MAP_GPIO_enableInterrupt(GPIO_PORT_P7, GPIO_PIN0);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    /*

    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
    
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster(); 

    switch_init();
    display_init();
    clear_display();
    displayString("It's Working!");

    while (1)
    {
      
      if (oldSwitch1!=newSwitch1 ||
          oldSwitch2!=newSwitch2 ||
          oldSwitch3!=newSwitch3 ||
          oldSwitch4!=newSwitch4 ||
          oldSwitch5!=newSwitch5 )
      {
        updateSwitches();
      }
    }
    
    return -1;
}

/* GPIO ISR */
void gpio_isr(void)
{
    uint32_t status;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    clear_display();
    displayString("Switch Flipped!");
    newSwitch1 = oldSwitch1^1;
}

/* Calendar Stuffs */
