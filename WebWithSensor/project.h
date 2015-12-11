#include <stdint.h>
#ifndef PROJECT_H //prevents header file from being included twice
#define PROJECT_H
	 
	//Function Prototypes
	void switch_init();
	void updateSwitches();

	void display_init();
	void clear_display();
	void entry_mode(int, int);
	void cd_shift(int, int);
	void display_on(int, int, int);
	void set_address(int, int, int, int, int, int, int);
	void function_set(int, int);
	void displayString(char*);
	void write(char, int);
	void clear_pins();
	void delayDisplay(int);

	int main(void);
	void gpio_isr(void);

#endif
