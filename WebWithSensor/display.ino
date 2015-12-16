#include "driverlib.h"
#include "project.h"

#include <stdint.h>
#include <stdbool.h>

/*
//Prototypes
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
void delay(int);
*/

void display_init()
{
  
  GPIO_setAsOutputPin(8, GPIO_PIN4);    //moving from 3.0 to 8.4
  GPIO_setAsOutputPin(8, GPIO_PIN2);    //moving from 5.7 to 8.2
  GPIO_setAsOutputPin(9, GPIO_PIN2);    //moving from 1.6 to 9.2
  GPIO_setAsOutputPin(6, GPIO_PIN2);    //moving from 5.6 to 6.2
  GPIO_setAsOutputPin(7, GPIO_PIN3);    //moving from 6.6 to 7.3
  GPIO_setAsOutputPin(7, GPIO_PIN1);    //moving from 6.7 to 7.1
  GPIO_setAsOutputPin(9, GPIO_PIN4);    //moving from 2.3 to 9.4
  GPIO_setAsOutputPin(9, GPIO_PIN6);    //moving from 5.1 to 9.6
  GPIO_setAsOutputPin(8, GPIO_PIN5);    //moving from 3.7 to 8.5
  GPIO_setAsOutputPin(9, GPIO_PIN0);    //moving from 3.5 to 9.0

  // 8 bit initialization process specified
  function_set(0, 0);
  function_set(0, 0);
  function_set(0, 0);
  function_set(1, 0);
  display_on(0, 0, 0);
  clear_display();
  entry_mode(1, 0);
  display_on(1, 1, 1);
}

/*clears the 2x16 display by writing a blank space to each address....right? and then sets
the cursor back to the beginning*/
// void clear_display(char* row2)
void clear_display()
{
  volatile unsigned int i;

  clear_pins();

  delay(250);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0);// Sets Enable to High so it will accept data

  delay(250);

  GPIO_setOutputHighOnPin(8, GPIO_PIN4);

  delay(250);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0); // Sets Enable to Low so that it will transmit data

  delay(250);
  /*
  set_address(0, 0, 0, 0, 0, 0, 0); //sets cursor to the first address/top left of screen

  for (i = 0; row2[i] != '\0'; i++) //writes a space in each address to clear the screen
  {
    write((row2[i]),1);
  }
  set_address(0, 0, 0, 0, 0, 0, 0); //sets the cursor/address to the first address (top left)
  for (i = 0; row2[i] != '\0'; i++) //writes a space in each address to clear the screen
  {
    write((row2[i]),1);
  }
  */
}

/*part of the display initialization process and enables data to be written to the display
if INC=1, it increments the cursor */
void entry_mode(int INC, int S)
{
  clear_pins();

  delay(50);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0); // Sets E to High

  delay(50);

  if (S)    GPIO_setOutputHighOnPin(8, GPIO_PIN4);
  if (INC)  GPIO_setOutputHighOnPin(8, GPIO_PIN2);
  GPIO_setOutputHighOnPin(9, GPIO_PIN2);

  delay(50);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0); // Sets E to Low

  delay(50);
}

void cd_shift(int SC, int RL)
{
  clear_pins();

  delay(50);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0); // Sets E to High

  delay(50);

  if (RL) GPIO_setOutputHighOnPin(9, GPIO_PIN2);
  if (SC) GPIO_setOutputHighOnPin(6, GPIO_PIN2);
  GPIO_setOutputHighOnPin(7, GPIO_PIN3);

  delay(50);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0); // Sets E to Low

  delay(50);
}

/*turns on the display and sets the cursor to the top right, blinking*/
void display_on(int ON, int CURSOR, int BLINK)
{
  clear_pins();

  delay(50);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0); // Sets E to High

  delay(50);

  if (BLINK)  GPIO_setOutputHighOnPin(8, GPIO_PIN4);
  if (CURSOR) GPIO_setOutputHighOnPin(8, GPIO_PIN2);
  if (ON)     GPIO_setOutputHighOnPin(9, GPIO_PIN2);
  GPIO_setOutputHighOnPin(6, GPIO_PIN2);

  delay(50);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0); // Sets E to Low

  delay(50);
}

void set_address(int D6, int D5, int D4, int D3, int D2, int D1, int D0)
{
  clear_pins();

  delay(250);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0); // Sets E to High

  delay(250);

  if (D0) GPIO_setOutputHighOnPin(8, GPIO_PIN4); //^
  if (D1) GPIO_setOutputHighOnPin(8, GPIO_PIN2); //|
  if (D2) GPIO_setOutputHighOnPin(9, GPIO_PIN2); //|
  if (D3) GPIO_setOutputHighOnPin(6, GPIO_PIN2); //|
  if (D4) GPIO_setOutputHighOnPin(7, GPIO_PIN3); //|
  if (D5) GPIO_setOutputHighOnPin(7, GPIO_PIN1); //|
  if (D6) GPIO_setOutputHighOnPin(9, GPIO_PIN4); //|
  GPIO_setOutputHighOnPin(9, GPIO_PIN6);         //v

  delay(250);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0); // Sets E to Low

  delay(250);
}

void function_set(int LINES, int FONT)
{
  clear_pins();

  delay(250);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0);   // Sets E to High

  delay(250);

  if (FONT)   GPIO_setOutputHighOnPin(9, GPIO_PIN2);
  if (LINES)  GPIO_setOutputHighOnPin(6, GPIO_PIN2);
  GPIO_setOutputHighOnPin(7, GPIO_PIN3);
  GPIO_setOutputHighOnPin(7, GPIO_PIN1);

  delay(250);

  GPIO_setOutputLowOnPin(9, GPIO_PIN0);   // Sets E to Low

  delay(250);
}

//prints the input string on the display
void displayString(char* c)
{
  volatile unsigned int i;
  for (i = 0; c[i] != '\0'; i++) //takes the array of characters and one-by-one prints them on the display.
  {
    write(c[i],1); //prints the current character on the display and shifts the cursor over one
    delay(100);
  }
}

//prints a character where the cursor currently is and shifts the cursor right one space
void write(char c, int RS)
{
  int data = (int) c; // Changes character into ASCII Value for write

  clear_pins();

  if (RS) GPIO_setOutputHighOnPin(8, GPIO_PIN5); // Sets RS to 1 if it is needed

  delay(10);

  GPIO_setOutputHighOnPin(9, GPIO_PIN0);   // Sets E to High

  delay(10);

  if (data & BIT0) GPIO_setOutputHighOnPin(8, GPIO_PIN4); //                ^
  if (data & BIT1) GPIO_setOutputHighOnPin(8, GPIO_PIN2); //                |
  if (data & BIT2) GPIO_setOutputHighOnPin(9, GPIO_PIN2); //Sets data to the character value passed in.
  if (data & BIT3) GPIO_setOutputHighOnPin(6, GPIO_PIN2); //                |
  if (data & BIT4) GPIO_setOutputHighOnPin(7, GPIO_PIN3); //                |
  if (data & BIT5) GPIO_setOutputHighOnPin(7, GPIO_PIN1); //                |
  if (data & BIT6) GPIO_setOutputHighOnPin(9, GPIO_PIN4); //                |
  if (data & BIT7) GPIO_setOutputHighOnPin(9, GPIO_PIN6); //                v

  GPIO_setOutputLowOnPin(9, GPIO_PIN0);   // Sets E to Low

  delay(10);
}

// clearup pins output
void clear_pins()
{
  GPIO_setOutputLowOnPin(8, GPIO_PIN4);
  GPIO_setOutputLowOnPin(8, GPIO_PIN2);
  GPIO_setOutputLowOnPin(9, GPIO_PIN2);
  GPIO_setOutputLowOnPin(6, GPIO_PIN2);
  GPIO_setOutputLowOnPin(7, GPIO_PIN3);
  GPIO_setOutputLowOnPin(7, GPIO_PIN1);
  GPIO_setOutputLowOnPin(9, GPIO_PIN4);
  GPIO_setOutputLowOnPin(9, GPIO_PIN6);
  GPIO_setOutputLowOnPin(9, GPIO_PIN0);
  GPIO_setOutputLowOnPin(8, GPIO_PIN5);
}

// Timer
/*
void delay(int count)
{
  int i, j;
  for (i = 0; i < count; ++i)
  {
    for (j = 0; j < 1000; ++j)
    {
    }
  }
}
*/

