/*
 If the IP address of your WiFi is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * CC3200 WiFi LaunchPad or CC3100 WiFi BoosterPack
   with TM4C or MSP430 LaunchPad

Grant Taylor, Beck Pang, Mason Possing
EE 478 final project
 */
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include "driverlib.h"
#include "project.h"
#include <WiFi.h>
#include <stdbool.h>
#include <idDHT11.h>


// your network name also called SSID
char ssid[] = "Beck's iPhone";
// your network password
char password[] = "robomasters";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

int idDHT11pin = 40; //Digital pin for comunications
int idDHT11intNumber = 40; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Temperature and humanidity sensor: http://playground.arduino.cc/Main/DHT11Lib
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

// Web server with CC3100
WiFiServer server(80);

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

//Bools for: breach, smoke, lowTemp, highTemp, moisture, wallPower
volatile bool breach;
volatile bool smoke;
volatile bool lowTemp;
volatile bool highTemp;
volatile bool moisture;
volatile bool wallPower;

//Storage for: Breach history (timestamp, 30 deep), Temp history (temp, 21 deep), 
// Moisture history (moisture, 21 deep), Smoke history (timestamp, 30 deep)
char* breaches[30];
char* temps[21];
char* moistures[21];
char* smokes[30];

// For sensors, Beck, Dec. 10th
char humidity[10];
char celsius[10];
char fahrenheit[10];
char kelvin[10];
char dewpoint[10];
char dewpointslow[10];


void setup() {
    display_init();
    clear_display();
    displayString("abcABC");
    
    Serial.begin(115200);      // initialize serial communication
/*******/
    /* Configuring P1.0 as an ouput for LED */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    //MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    //MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    //MAP_Interrupt_enableInterrupt(INT_PORT1);
    
    //pin 2.7(40) interrupt for temp and humanidity sensor
    //pin 2.6(39) 2.4(38) for door/window interrupt
    // MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    pinMode(40, INPUT_PULLUP);
    attachInterrupt(39, gpio_isr, FALLING);
    attachInterrupt(38, gpio_isr, FALLING);
    
/*******/
    
    // attempt to connect to Wifi network:
    displayString("Attempting to connect to Network named: ");
    // print the network name (SSID);
    displayString(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED) {
        // print dots while we wait to connect
        Serial.print(".");
        displayString(".");
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
                    
                    generateHTML(client);
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
        
            handleURLRequest(buffer);
        }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    }
  
    DHT11.acquire();
    while (DHT11.acquiring())
    ;
    int result = DHT11.getStatus();
    switch (result)
    {
    case IDDHTLIB_OK: 
        Serial.println("OK"); 
        break;
    case IDDHTLIB_ERROR_CHECKSUM: 
        Serial.println("Error\n\r\tChecksum error"); 
        break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT: 
        Serial.println("Error\n\r\tISR Time out error"); 
        break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
        Serial.println("Error\n\r\tResponse time out error"); 
        break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT: 
        Serial.println("Error\n\r\tData time out error"); 
        break;
    case IDDHTLIB_ERROR_ACQUIRING: 
        Serial.println("Error\n\r\tAcquiring"); 
        break;
    case IDDHTLIB_ERROR_DELTA: 
        Serial.println("Error\n\r\tDelta time to small"); 
        break;
    case IDDHTLIB_ERROR_NOTSTARTED: 
        Serial.println("Error\n\r\tNot started"); 
        break;
    default: 
        Serial.println("Unknown error"); 
        break;
    }

    displayString("Humidity (%): ");
    sprintf(humidity, "%f", DHT11.getHumidity());
    displayString(humidity);
    
    displayString("Temperature (oC): ");
    sprintf(celsius, "%f", DHT11.getCelsius());
    displayString(celsius);
    
    displayString("Temperature (oF): ");
    sprintf(fahrenheit, "%f", DHT11.getFahrenheit());
    displayString(fahrenheit);
    
    displayString("Temperature (K): ");
    sprintf(kelvin, "%f", DHT11.getKelvin());
    displayString(kelvin);
    
    displayString("Dew Point (oC): ");
    sprintf(dewpoint, "%f", DHT11.getDewPoint());
    displayString(dewpoint);
    
    displayString("Dew Point Slow (oC): ");
    sprintf(dewpointslow, "%f", DHT11.getDewPointSlow());
    displayString(dewpointslow);
    
    delay(2000);
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

void generateHTML(WiFiClient client) {
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
    
    // Sensors readouts
    client.print("Humidity (%):  ");
    client.print(humidity);
    client.println("<br>");
    client.print("Temperature (oC):  ");
    client.print(celsius);
    client.println("<br>");
    client.print("Temperature (oF): ");
    client.print(fahrenheit);
    client.println("<br>");
    client.print("Temperature (K): ");
    client.print(kelvin);
    client.println("<br>");
    client.print("Dew Point (oC): ");
    client.print(dewpoint);
    client.println("<br>");
    client.print("Dew Point Slow (oC): ");
    client.print(dewpointslow);
    client.println("<br>");

    // The HTTP response ends with another blank line:
    client.println("</body>");
}

void handleURLRequest(char buffer[150]) {
    // Check to see if the client request was "GET /H" or "GET /L":
    if (endsWith(buffer, "GET /LED/H")) {
      MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);            // GET /H turns the LED on
    }
    if (endsWith(buffer, "GET /LED/L")) {
      MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);            // GET /L turns the LED off
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

/* GPIO ISR */
void gpio_isr(void)
{
    //uint32_t status;
    //status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    //MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    newSwitch1 = oldSwitch1^1;
    newSwitch2 = oldSwitch2^1;
    newSwitch3 = oldSwitch3^1;
    newSwitch4 = oldSwitch4^1;
    newSwitch5 = oldSwitch5^1;
    if (oldSwitch1!=newSwitch1 ||
          oldSwitch2!=newSwitch2 ||
          oldSwitch3!=newSwitch3 ||
          oldSwitch4!=newSwitch4 ||
          oldSwitch5!=newSwitch5 )
      {
        updateSwitches();
      }
}

/* GPIO ISR for Temperature and Humanity Sensor */
void dht11_wrapper() {
    DHT11.isrCallback();
}
