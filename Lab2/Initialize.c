// EE 478 2015 Autumn
#include <stdio.h>
#include <stdlib.h>
#include <p18f25k22.h>
#include <timers.h>
#include <usart.h>
#include <adc.h>
#include <delays.h>
#include "lab2.h"

#define BAUD_9600_20MHz 129

// PORT A, 
// PIN A0 is tempIn, A1 is bp1In, A2 is bp2In, A3 is pulseIn
//     A4 is USART/RS232 Tx, A5 is USART/RS232 Rx, A7 is OscIn
// PORT B, all output for display data
// PORT C is 
// PIN C0 is RS, C1 is R/W, C2 is E, enable
// PIN C6 is UART Tx, C7 is UART Rx

void display_init();
void delay(int aDelay);
void write(char, int);
void clear_display(const char*);
void display_on(int, int, int);
void function_set(int, int);
void entry_mode(int, int);
void cd_shift(int, int);
void set_address(int, int, int, int, int, int, int);
void clear_pins();
void UART_init();
void display_string1(char*);

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
}

void ADC_init(){
    /*
    OpenADC((char) (ADC_FOSC_RC & 
            ADC_RIGHT_JUST & 
            ADC_3ANA_0REF), 
            (char) (ADC_CH0 & ADC_INT_OFF));
    */
}

void spi_init(){
    //TODO:
    //OpenSPI(); //insert args
    //MISO/SDO C5
    //MISO/SDI C4
    //SCK C3
    //nSS A5
}

void UART_init(){
    // PIN A0 is  tempIn, A1 is bp1In, A2 is bp2In, A3 is pulseIn
    
    //C6 is USART/RS232 Tx, C7 is USART/RS232 Rx
    
    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input
    
    // Enable digital for all c pins
    ANSELC=0x00;
    //LATC  =0x00;
    //TRISC =0x00;
    
    RCONbits.IPEN = 1; // turn on priorities
    IPR1bits.RCIP = 1; // make Rx interrupt high priority
    INTCONbits.GIEH = 1; // turn on high priority interrupts
    
    Open1USART(USART_TX_INT_OFF  &
            USART_RX_INT_OFF   &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT   &
            //USART_CONT_RX     &
            USART_BRGH_HIGH, 
            BAUD_9600_20MHz);   //Set Baud Rate here  Fosc / (64 * (spbrg + 1))
}

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
void clear_display(const char* row2)
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