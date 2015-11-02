/* 
 * File:   lab2.h
 * Author: Grant
 *
 * Created on October 24, 2015, 1:26 PM
 */

#ifndef SLAVE_H
#define	SLAVE_H

 	// For display_init
 	void clear_display(char*);
 	void display_on(int, int, int);
	void function_set(int, int);
	void entry_mode(int, int);

	void clear_pins();
	void write(char, int);
	void delay(int aDelay);

	// void cd_shift(int, int);
	// void set_address(int, int, int, int, int, int, int);

	// void interrupt_at_high_vector(void);
	
	void display_string1(char*);

	// For initialize
	void display_init();
	void Timer_init();
	void ADC_init();
	void spi_init();

	// For measure funct
	int getTemp();
	int bloodPres1();
	int bloodPres2();
	int pulseRate();

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SLAVE_H */
