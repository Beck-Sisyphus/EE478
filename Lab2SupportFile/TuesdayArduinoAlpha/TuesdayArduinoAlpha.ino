/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 12 (connect to TX of other device)
 * TX is digital pin 13 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
 
#define rxPin 12
#define txPin 13
#include <SoftwareSerial.h>

SoftwareSerial mySerial (rxPin, txPin);// RX, TX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Goodnight moon!");
  
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() // run over and over
{
  char command;
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    command = Serial.read();
    if (command == 'p') {
      mySerial.write('p');
      Serial.write("pulse rate measurement starts");
    }
    else if (command == 's') {
      mySerial.write('s');
      Serial.write("systolic pressure measurement starts");
    }
    else if (command == 'd') {
      mySerial.write('d');
      Serial.write("diastolic pressure measurement");
    } 
    else if (command == 't') {
      mySerial.write('t');
      Serial.write("temperature reading starts");
    } 
    else {
      Serial.write("Invalid command");
    }
    Serial.println();
  }
    
  // Proper command include p, s, d, t
  // p command trigger pulse rate reading
  // s command trigger systolic pressure
  // d command trigger diastolic pressure
  // t command trigger temperature reading
}
