/* ***************************************************************************
**  File Name    : picspi.c
**  Version      : 1.0
**  Description  : SPI I/O Using Microchip MCP23S17 16-Bit I/O Expander
**  Author       : RWB
**  Target       : PICJazz 20PIN Board: PIC18F14K22
**  Compiler     : Microchip C18 v3.36 C Compiler
**  IDE          : Microchip MPLAB IDE v8.46
**  Programmer   : PICKit3, Firmware Suite Version 01.25.20
**  Last Updated : 12 Aug 2010
** ***************************************************************************/
#include <p18cxxx.h>
#include <delays.h>
/*
** PIC18F14K22 Configuration Bit:
**
** FOSC = IRC        - Internal RC Oscillator
** PLLEN = OFF       - PLL is under software control
** PCLKEN = ON       - Primary Clock Enable
** FCMEN = OFF       - Fail-Safe Clock Monitor disabled
** PWRTEN = OFF      - Power Up Timer disabled
** BOREN = OFF       - Brown-out Reset disabled in hardware and software
** WDTEN = OFF       - WDT is controlled by SWDTEN bit of the WDTCON register
** MCLRE = ON        - MCLR pin enabled, RE3 input pin disabled
** LVP = OFF         - Single-Supply ICSP disabled
*/
#pragma config FOSC = IRC, PLLEN = OFF, PCLKEN = ON
#pragma config FCMEN = OFF, BOREN = OFF, PWRTEN = OFF
#pragma config WDTEN = OFF, MCLRE = ON, LVP = OFF
// MCP23S17 SPI Slave Device
#define SPI_SLAVE_ID    0x40
#define SPI_SLAVE_ADDR  0x00      // A2=0,A1=0,A0=0
#define SPI_SLAVE_WRITE 0x00
#define SPI_SLAVE_READ  0x01
// MCP23S17 Registers Definition for BANK=0 (default)
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCONA 0x0A
#define GPPUA  0x0C
#define GPPUB  0x0D
#define GPIOA  0x12
#define GPIOB  0x13
static rom unsigned char led_patern[32] = {0b00000001,0b00000011,0b00000110,0b00001100,0b00011001,  	 			                          0b00110011,0b01100110,0b11001100,0b10011000,0b00110000,				                    0b01100000,0b11000000,0b10000000,0b00000000,0b00000000,				                    0b00000000,0b10000000,0b11000000,0b01100000,0b00110000,				                    0b10011000,0b11001100,0b01100110,0b00110011,0b00011001,				                    0b00001100,0b00000110,0b00000011,0b00000001,0b00000000,
0b00000000,0b00000000};
// Delay in 1 ms (approximately) for 16 MHz Internal Clock
void delay_ms(unsigned int ms)
{
  do {
    Delay1KTCYx(4);
  } while(--ms);
}
void SPI_Write(unsigned char addr,unsigned char data)
{
  // Activate the SS SPI Select pin
  PORTCbits.RC6 = 0;
  // Start MCP23S17 OpCode transmission
  SSPBUF = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_WRITE;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);
  // Start MCP23S17 Register Address transmission
  SSPBUF = addr;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);            

  // Start Data transmission
  SSPBUF = data;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);
  // CS pin is not active
  PORTCbits.RC6 = 1;
}
unsigned char SPI_Read(unsigned char addr)
{
  // Activate the SS SPI Select pin
  PORTCbits.RC6 = 0;
  // Start MCP23S17 OpCode transmission
  SSPBUF = SPI_SLAVE_ID | ((SPI_SLAVE_ADDR << 1) & 0x0E)| SPI_SLAVE_READ;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);
  // Start MCP23S17 Address transmission
  SSPBUF = addr;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);  

  // Send Dummy transmission for reading the data
  SSPBUF = 0x00;
  // Wait for Data Transmit/Receipt complete
  while(!SSPSTATbits.BF);  

  // CS pin is not active
  PORTCbits.RC6 = 1;
  return(SSPBUF);
}
void main(void)
{
  unsigned char cnt,togbutton,inp;
  unsigned int idelay;

  OSCCON=0x70;         // Select 16 MHz internal clock
  TRISC = 0x00;        // Set All on PORTC as Output
  TRISA = 0x30;        // Input for RA4 and RA5
  TRISB = 0x00;
  ANSEL = 0x08;        // Set PORT AN3 to analog input
  ANSELH = 0x00;       // Set PORT AN8 to AN11 as Digital I/O
  /* Init the PIC18F14K22 ADC Peripheral */
  ADCON0=0b00001101;   // ADC port channel 3 (AN3), Enable ADC
  ADCON1=0b00000000;   // Use Internal Voltage Reference (Vdd and Vss)
  ADCON2=0b10101011;   // Right justify result, 12 TAD, Select the FRC for 16 MHz
  /* Initial the PIC18F14K22 SPI Peripheral */
  TRISCbits.TRISC6 = 0;  // RC6/SS - Output (Chip Select)
  TRISCbits.TRISC7= 0;   // RC7/SDO - Output (Serial Data Out)
  TRISBbits.TRISB4= 1;   // RB4/SDI - Input (Serial Data In)
  TRISBbits.TRISB6= 0;   // RB6/SCK - Output (Clock)

  SSPSTAT = 0x40;        // Set SMP=0 and CKE=1. Notes: The lower 6 bit is read only
  SSPCON1 = 0x20;        // Enable SPI Master with Fosc/4
  PORTCbits.RC6 = 1;     // Disable Chip Select
  // Initial the MCP23S17 SPI I/O Expander
  SPI_Write(IOCONA,0x28);   // I/O Control Register: BANK=0, SEQOP=1, HAEN=1 (Enable Addressing)
  SPI_Write(IODIRA,0x00);   // GPIOA As Output
  SPI_Write(IODIRB,0xFF);   // GPIOB As Input
  SPI_Write(GPPUB,0xFF);    // Enable Pull-up Resistor on GPIOB
  SPI_Write(GPIOA,0x00);    // Reset Output on GPIOA
  // Initial Variable Used
  togbutton=0;              // Toggle Button
  cnt=0;
  idelay=100;               // Default Delay;   

  for(;;) {
    inp=SPI_Read(GPIOB);    // Read from GPIOB
	if (inp == 0xFE) {      // Button is pressed
	  delay_ms(1);		 

	  inp=SPI_Read(GPIOB);  // Read from GPIOB, for simple debounce
	  if (inp == 0xFE) togbutton^=0x01;
	  if (togbutton == 0x00) {
	    SPI_Write(GPIOA,0x00); // Write to MCP23S17 GPIOA
	    cnt=0;
      }
	}
    if (togbutton) {
      ADCON0bits.GO=1;
      while (ADCON0bits.GO);   // Wait conversion done
      idelay=ADRESL;           // Get the 8 bit LSB result
      idelay += (ADRESH << 8); // Get the 2 bit MSB result   

      // Write to GPIOA
      SPI_Write(GPIOA,led_patern[cnt++]);
      if(cnt >= 32) cnt=0;
    }
    delay_ms(idelay);          // Call Delay function
  }
}
/* EOF: picspi.c */