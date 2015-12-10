/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your WiFi is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * CC3200 WiFi LaunchPad or CC3100 WiFi BoosterPack
   with TM4C or MSP430 LaunchPad

 created 25 Nov 2012
 by Tom Igoe
 modified 6 July 2014
 by Noah Luskey
 */
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
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

// your network name also called SSID
char ssid[] = "Beck's iPhone";
// your network password
char password[] = "robomasters";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the LED pin mode

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");

  /********  Firmware Level Code from CCS   *********/ 
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

    /*Initialize Sensor Ports  2.4-2.7   In Progress!!! */
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN4);

    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN5);

    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);

    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN7);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN7);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN7);

    MAP_Interrupt_enableInterrupt(INT_PORT2);


    /* Enabling SRAM Bank Retention */
    MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
    
    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster(); 

    switch_init();
    display_init();
    clear_display();
    displayString("abcABC");
    /********  Firmware Level code from CCS end *******/ 
}

void loop() {
  int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<html><head><title>Energia CC3200 WiFi Web Server</title></head><body align=center>");
            client.println("<h1 align=center><font color=\"red\">Welcome to the CC3200 WiFi Web Server</font></h1>");
            client.print("RED LED <button onclick=\"location.href='/LED/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/LED/L'\">LOW</button><br>");

            // Need to control 5 remotes, and 3 alarms
            client.print("Remote I <button onclick=\"location.href='/RemoteI/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/RemoteI/L'\">LOW</button><br>");
            client.print("Remote II <button onclick=\"location.href='/RemoteII/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/RemoteII/L'\">LOW</button><br>");
            client.print("Remote III <button onclick=\"location.href='/RemoteIII/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/RemoteIII/L'\">LOW</button><br>");
            client.print("Remote IV <button onclick=\"location.href='/RemoteIV/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/RemoteIV/L'\">LOW</button><br>");
            client.print("Remote V <button onclick=\"location.href='/RemoteV/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/RemoteV/L'\">LOW</button><br>");

            client.print("Smoke Alarm <button onclick=\"location.href='/AlarmSmoke/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/AlarmSmoke/L'\">LOW</button><br>");
            client.print("Broker Alarm <button onclick=\"location.href='/AlarmBroker/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/AlarmBroker/L'\">LOW</button><br>");
            client.print("Temp Alarm <button onclick=\"location.href='/AlarmTemp/H'\">HIGH</button>");
            client.println(" <button onclick=\"location.href='/AlarmTemp/L'\">LOW</button><br>");

            // The HTTP response ends with another blank line:
            client.println("</body>");
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        // @@ Handler:
        // Check to see if the client request was "GET /H" or "GET /L":
        if (endsWith(buffer, "GET /LED/H")) {
          digitalWrite(RED_LED, HIGH);               // GET /H turns the LED on
        }
        if (endsWith(buffer, "GET /LED/L")) {
          digitalWrite(RED_LED, LOW);                // GET /L turns the LED off
        }

        // Remote Handler:
        if (endsWith(buffer, "GET /RemoteI/H")) {
            // First Remote Control digital pin, high
            newSwitch1 = true;
        }
        if (endsWith(buffer, "GET /RemoteI/L")) {
            // First Remote Control digital pin, low
            newSwitch1 = false;
        }
    		if (endsWith(buffer, "GET /RemoteII/H")) {
    			  // Second Remote Control digital pin, high
            newSwitch2 = true;
    		}
    		if (endsWith(buffer, "GET /RemoteII/L")) {
    			  // Second Remote Control digital pin, low
            newSwitch2 = false;
    		}
    		if (endsWith(buffer, "GET /RemoteIII/H")) {
    			  // Third Remote Control digital pin, high
            newSwitch3 = true;
    		}
    		if (endsWith(buffer, "GET /RemoteIII/L")) {
    			  // Third Remote Control digital pin, low
            newSwitch3 = false;
    		}
    		if (endsWith(buffer, "GET /RemoteIV/H")) {
    			  // Fourth Remote Control digital pin, high
            newSwitch4 = true;
    		}
    		if (endsWith(buffer, "GET /RemoteIV/L")) {
    			  // Fourth Remote Control digital pin, low
            newSwitch4 = false;
    		}
    		if (endsWith(buffer, "GET /RemoteV/H")) {
    			  // Fifth Remote Control digital pin, high
            newSwitch5 = true;
    		}
    		if (endsWith(buffer, "GET /RemoteV/L")) {
    			  // Fifth Remote Control digital pin, low
            newSwitch5 = false;
    		}

    		// Turn on or off the alarm
    		if (endsWith(buffer, "GET /AlarmSmoke/H")) {
    			// Fifth Remote Control digital pin, high
    		}
    		if (endsWith(buffer, "GET /AlarmSmoke/L")) {
    			// Fifth Remote Control digital pin, low
    		}
    		if (endsWith(buffer, "GET /AlarmBroker/H")) {
    			// Fifth Remote Control digital pin, high
    		}
    		if (endsWith(buffer, "GET /AlarmBroker/L")) {
    			// Fifth Remote Control digital pin, low
    		}
    		if (endsWith(buffer, "GET /AlarmTemp/H")) {
    			// Fifth Remote Control digital pin, high
    		}
    		if (endsWith(buffer, "GET /AlarmTemp/L")) {
    			// Fifth Remote Control digital pin, low
    		}
      }
    }

    if (oldSwitch1!=newSwitch1 ||
        oldSwitch2!=newSwitch2 ||
        oldSwitch3!=newSwitch3 ||
        oldSwitch4!=newSwitch4 ||
        oldSwitch5!=newSwitch5 )
    {
        updateSwitches();
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

/* GPIO ISR */
void gpio_isr(void)
{
    uint32_t status;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    newSwitch1 = oldSwitch1^1;
    //newSwitch2 = oldSwitch2^1;
    //newSwitch3 = oldSwitch3^1;
    //newSwitch4 = oldSwitch4^1;
    //newSwitch5 = oldSwitch5^1;
}

