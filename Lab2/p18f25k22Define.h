/* 
 * File:   lab2.h
 * Author: Grant
 *
 * Created on October 24, 2015, 1:26 PM
 */

#ifndef LAB2_H
#define	LAB2_H

    void display_init();
    //  #pragma config PLLDIV = 5, CPUDIV = OSC1_PLL2, USBDIV = 2
	// OR:
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

	void delay(int aDelay);
	void write(char, int);
	void clear_display(char*);
	void display_on(int, int, int);
	void function_set(int, int);
	void entry_mode(int, int);
	void cd_shift(int, int);
	void set_address(int, int, int, int, int, int, int);
	void rxISR(void);
	void interrupt_at_high_vector(void);
	void display_init();
	void display_string1(char*);
	void UART_init();
	int temp();

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LAB2_H */
