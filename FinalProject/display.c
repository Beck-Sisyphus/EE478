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
  GPIO_setAsOutputPin(2, GPIO_PIN7);
  GPIO_setAsOutputPin(2, GPIO_PIN6);
  GPIO_setAsOutputPin(2, GPIO_PIN4);
  GPIO_setAsOutputPin(5, GPIO_PIN6);
  GPIO_setAsOutputPin(6, GPIO_PIN6);
  GPIO_setAsOutputPin(6, GPIO_PIN7);
  GPIO_setAsOutputPin(2, GPIO_PIN3);
  GPIO_setAsOutputPin(5, GPIO_PIN1);
  GPIO_setAsOutputPin(3, GPIO_PIN5);
  GPIO_setAsOutputPin(3, GPIO_PIN7);

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

  delay(25);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5);// Sets Enable to High so it will accept data

  delay(25);

  GPIO_setOutputHighOnPin(2, GPIO_PIN7);

  delay(25);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5); // Sets Enable to Low so that it will transmit data

  delay(25);
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

  delay(5);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5); // Sets E to High

  delay(5);

  if (S)    GPIO_setOutputHighOnPin(2, GPIO_PIN7);
  if (INC)  GPIO_setOutputHighOnPin(2, GPIO_PIN6);
  GPIO_setOutputHighOnPin(2, GPIO_PIN4);

  delay(5);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5); // Sets E to Low

  delay(5);
}

void cd_shift(int SC, int RL)
{
  clear_pins();

  delay(5);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5); // Sets E to High

  delay(5);

  if (RL) GPIO_setOutputHighOnPin(2, GPIO_PIN4);
  if (SC) GPIO_setOutputHighOnPin(5, GPIO_PIN6);
  GPIO_setOutputHighOnPin(6, GPIO_PIN6);

  delay(5);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5); // Sets E to Low

  delay(5);
}

/*turns on the display and sets the cursor to the top right, blinking*/
void display_on(int ON, int CURSOR, int BLINK)
{
  clear_pins();

  delay(5);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5); // Sets E to High

  delay(5);

  if (BLINK)  GPIO_setOutputHighOnPin(2, GPIO_PIN7);
  if (CURSOR) GPIO_setOutputHighOnPin(2, GPIO_PIN6);
  if (ON)     GPIO_setOutputHighOnPin(2, GPIO_PIN4);
  GPIO_setOutputHighOnPin(5, GPIO_PIN6);

  delay(5);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5); // Sets E to Low

  delay(5);
}

void set_address(int D6, int D5, int D4, int D3, int D2, int D1, int D0)
{
  clear_pins();

  delay(25);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5); // Sets E to High

  delay(25);

  if (D0) GPIO_setOutputHighOnPin(2, GPIO_PIN7); //^
  if (D1) GPIO_setOutputHighOnPin(2, GPIO_PIN6); //|
  if (D2) GPIO_setOutputHighOnPin(2, GPIO_PIN4); //|
  if (D3) GPIO_setOutputHighOnPin(5, GPIO_PIN6); //|
  if (D4) GPIO_setOutputHighOnPin(6, GPIO_PIN6); //|
  if (D5) GPIO_setOutputHighOnPin(6, GPIO_PIN7); //|
  if (D6) GPIO_setOutputHighOnPin(2, GPIO_PIN3); //|
  GPIO_setOutputHighOnPin(5, GPIO_PIN1);         //v

  delay(25);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5); // Sets E to Low

  delay(25);
}

void function_set(int LINES, int FONT)
{
  clear_pins();

  delay(25);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5);   // Sets E to High

  delay(25);

  if (FONT)   GPIO_setOutputHighOnPin(2, GPIO_PIN4);
  if (LINES)  GPIO_setOutputHighOnPin(5, GPIO_PIN6);
  GPIO_setOutputHighOnPin(6, GPIO_PIN6);
  GPIO_setOutputHighOnPin(6, GPIO_PIN7);

  delay(25);

  GPIO_setOutputLowOnPin(3, GPIO_PIN5);   // Sets E to Low

  delay(25);
}

//prints the input string on the display
void displayString(char* c)
{
  volatile unsigned int i;
  for (i = 0; c[i] != '\0'; i++) //takes the array of characters and one-by-one prints them on the display.
  {
    write(c[i],1); //prints the current character on the display and shifts the cursor over one
    delay(10);
  }
}

//prints a character where the cursor currently is and shifts the cursor right one space
void write(char c, int RS)
{
  int data = (int) c; // Changes character into ASCII Value for write

  clear_pins();

  if (RS) GPIO_setOutputHighOnPin(3, GPIO_PIN7); // Sets RS to 1 if it is needed

  delay(1);

  GPIO_setOutputHighOnPin(3, GPIO_PIN5);   // Sets E to High

  delay(1);

  if (data & BIT0) GPIO_setOutputHighOnPin(2, GPIO_PIN7); //                ^
  if (data & BIT1) GPIO_setOutputHighOnPin(2, GPIO_PIN6); //                |
  if (data & BIT2) GPIO_setOutputHighOnPin(2, GPIO_PIN4); //Sets data to the character value passed in.
  if (data & BIT3) GPIO_setOutputHighOnPin(5, GPIO_PIN6); //                |
  if (data & BIT4) GPIO_setOutputHighOnPin(6, GPIO_PIN6); //                |
  if (data & BIT5) GPIO_setOutputHighOnPin(6, GPIO_PIN7); //                |
  if (data & BIT6) GPIO_setOutputHighOnPin(2, GPIO_PIN3); //                |
  if (data & BIT7) GPIO_setOutputHighOnPin(5, GPIO_PIN1); //                v

  GPIO_setOutputLowOnPin(3, GPIO_PIN5);   // Sets E to Low

  delay(1);
}

// clearup pins output
void clear_pins()
{
  GPIO_setOutputLowOnPin(2, GPIO_PIN7);
  GPIO_setOutputLowOnPin(2, GPIO_PIN6);
  GPIO_setOutputLowOnPin(2, GPIO_PIN4);
  GPIO_setOutputLowOnPin(5, GPIO_PIN6);
  GPIO_setOutputLowOnPin(6, GPIO_PIN6);
  GPIO_setOutputLowOnPin(6, GPIO_PIN7);
  GPIO_setOutputLowOnPin(2, GPIO_PIN3);
  GPIO_setOutputLowOnPin(5, GPIO_PIN1);
  GPIO_setOutputLowOnPin(3, GPIO_PIN5);
  GPIO_setOutputLowOnPin(3, GPIO_PIN7);
}

// Timer
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
