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

/* function prototypes*/
extern void clear_display(char*);
extern void display_on(int, int, int);
extern void function_set(int, int);
extern void entry_mode(int, int);

extern void clear_pins();
extern void write(char, int);
extern void delay(int aDelay);
//void cd_shift(int, int);
//void set_address(int, int, int, int, int, int, int);

/*initializes display and configures ports on stellaris board to
corresponding pins on 2.16 display */
void display_init()
{
  // gpio initialization
  TRISB = 0x00;
  TRISC = 0b10000000;
  
  //input init
  //LATAbits.LATA0 = 0; //enable A1 for temp
  
  // 8 bit initialization process specified
  function_set(0, 0);
  function_set(0, 0);
  function_set(0, 0);
  function_set(1, 0);
  display_on(0, 0, 0);
  clear_display("");
  entry_mode(1, 0);
  display_on(1, 1, 1);
}


/*clears the 2x16 display by writing a blank space to each address....right? and then sets 
the cursor back to the beginning*/
void clear_display(char* row2)
{
  volatile unsigned int i;
  
  clear_pins();

  delay(25);
  
  LATCbits.LATC1 = 1;   // Sets Enable to High so it will accept data
  
  delay(25);
  
  LATBbits.LATB0 = 1;                
  
  delay(25);
  
  LATCbits.LATC1 = 0; // Sets Enable to Low so that it will transmit data
  
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
  
  LATCbits.LATC1 = 1; // Sets E to High
  
  delay(5);
  
  if (S)    LATBbits.LATB0 = 1;
  if (INC)  LATBbits.LATB1 = 1;  
  LATBbits.LATB2 = 1;                
  
  delay(5);
  
  LATCbits.LATC1 = 0; // Sets E to Low
  
  delay(5);
}

void cd_shift(int SC, int RL)
{
  clear_pins();
  
  delay(5);
  
  LATCbits.LATC1 = 1; // Sets E to High
  
  delay(5);
  
  if (RL) LATBbits.LATB2 = 1;
  if (SC) LATBbits.LATB3 = 1;  
  LATBbits.LATB4 = 1;                
  
  delay(5);
  
  LATCbits.LATC1 = 0; // Sets E to Low
  
  delay(5);
}

/*turns on the display and sets the cursor to the top right, blinking*/ 
void display_on(int ON, int CURSOR, int BLINK)
{
  clear_pins();
  
  delay(5);
  
  LATCbits.LATC1 = 1; // Sets E to High
  
  delay(5);
  
  if (BLINK)  LATBbits.LATB0 = 1;
  if (CURSOR) LATBbits.LATB1 = 1;
  if (ON)     LATBbits.LATB2 = 1;
  LATBbits.LATB3 = 1;              
  
  delay(5);
  
  LATCbits.LATC1 = 0; // Sets E to Low
  
  delay(5);
}

/**/
void set_address(int D6, int D5, int D4, int D3, int D2, int D1, int D0)
{
  clear_pins();
  
  delay(25);
  
  LATCbits.LATC1 = 1; // Sets E to High
  
  delay(25);

  LATBbits.LATB0 = D0;
  LATBbits.LATB1 = D1;   
  LATBbits.LATB2 = D2;   
  LATBbits.LATB3 = D3;   
  LATBbits.LATB4 = D4;   
  LATBbits.LATB5 = D5;   
  LATBbits.LATB6 = D6;   
  LATBbits.LATB7 = 1;
  
  delay(25);
  
  LATCbits.LATC1 = 0; // Sets E to Low
  
  delay(25);
}

void function_set(int LINES, int FONT)
{
  clear_pins();
  
  delay(25);
  
  LATCbits.LATC1 = 1;   // Sets E to High
  
  delay(25);
  
  if (FONT)   LATBbits.LATB2 = 1;
  if (LINES)  LATBbits.LATB3 = 1;
  LATBbits.LATB4 = 1;
  LATBbits.LATB5 = 1;             
  
  delay(25);
  
  LATCbits.LATC1 = 0;   // Sets E to Low
  
  delay(25);
}

//prints the input string on the display
void display_string1(char* c)
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
  
  if (RS) LATCbits.LATC0 = 1; // Sets RS to 1 if it is needed
  
  delay(1);
  
  LATCbits.LATC1 = 1;   // Sets E to High
  
  delay(1);
  
  // if (data & 0x01) GPIO_PORTB_DATA_R |= 0x01; //                ^
  // if (data & 0x02) GPIO_PORTB_DATA_R |= 0x02; //                |
  // if (data & 0x04) GPIO_PORTB_DATA_R |= 0x04; //Sets data to the character value passed in.
  // if (data & 0x08) GPIO_PORTB_DATA_R |= 0x08; //                |
  // if (data & 0x10) GPIO_PORTB_DATA_R |= 0x10; //                |
  // if (data & 0x20) GPIO_PORTB_DATA_R |= 0x20; //                |
  // if (data & 0x40) GPIO_PORTB_DATA_R |= 0x40; //                |
  // if (data & 0x80) GPIO_PORTC_DATA_R |= 0x80; //                v
  
  if (data>=128){
      LATBbits.LATB7 = 1;
      data = data - 128;
  }
  if (data>=64){
      LATBbits.LATB6 = 1;
      data = data - 64;
  }
  if (data>=32){
      LATBbits.LATB5 = 1;
      data = data - 32;
  }
  if (data>=16){
      LATBbits.LATB4 = 1;
      data = data - 16;
  }
  if (data>=8){
      LATBbits.LATB3 = 1;
      data = data - 8;
  }
  if (data>=4){
      LATBbits.LATB2 = 1;
      data = data - 4;
  }
  if (data>=2){
      LATBbits.LATB1 = 1;
      data = data - 2;
  }
  if (data>=1){
      LATBbits.LATB0 = 1;
  }
  data = 0;
  LATCbits.LATC1 = 0;   // Sets E to Low
  
  delay(1);
}

// Timer
void delay(int count) 
{
  int i, j;
  for (i = 0; i < count; ++i)
  {
    for (j = 0; j < 50; ++j)
    {
    }
  }
} 

// clearup pins output
void clear_pins() 
{
  // PORT B, all output for display data
  // PORT C0 to C2, output for control signal
  // PORT C0 is RS, C1 is E
  LATBbits.LATB0 = 0;
  LATBbits.LATB1 = 0;
  LATBbits.LATB2 = 0;
  LATBbits.LATB3 = 0;
  LATBbits.LATB4 = 0;
  LATBbits.LATB5 = 0;
  LATBbits.LATB6 = 0;
  LATBbits.LATB7 = 0;
  LATCbits.LATC0 = 0;
  LATCbits.LATC1 = 0;
}