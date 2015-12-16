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
#include <SPI.h>
#endif
#include "driverlib.h"
#include "project.h"
#include "DateTimeLibrary.h"
#include "msp.h"
#include <WiFi.h>
#include <stdbool.h>
#include <idDHT11.h>

//WIFI Parameters
// your network name also called SSID
char ssid[] = "Grant's iPhone";
// your network password
char password[] = "bocco7744";
// your network key Index number (needed only for WEP)
int keyIndex = 0;
// Web server with CC3100
WiFiServer server(80);


//Temp/Moisture Initialization
//interrupt declaration
int idDHT11pin = 39; //Digital pin for comunications
int idDHT11intNumber = 39; //interrupt number (must be the one that use the previus defined pin (see table above)
void dht11_wrapper(); // must be declared before the lib initialization

// Temperature and humanidity sensor: http://playground.arduino.cc/Main/DHT11Lib
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

//Display Prototypes
extern void display_init();
extern void clear_display();
extern void displayString(char*);

//Wifi Prototypes
void printWifiStatus();
uint32_t sendNTPpacket(IPAddress& address);

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

//Bools for: breach, smoke, lowTemp, highTemp, moisture, wallPower
bool breach = false;
bool lastBreach = false;
bool smoke = false;
bool lastSmoke = false;

//RTC Variables
time_t myEpochRTC;
tm myStructureRTC;
tm temp;
DateTime myRTC;
bool flagRTC = true;

//Schedule Variables
time_t schedule[256];
int valid = 1;
int z;
time_t action;
int month;
int day;
int year;
int hours;
int minutes;
char recur;

//Storage for: Breach history (timestamp, 30 deep), Temp history (temp, 21 deep), 
// Moisture history (moisture, 21 deep), Smoke history (timestamp, 30 deep)
int Dcounter = 0;
int Scounter = 0;
time_t doorLog[20];
time_t smokeLog[20];

//Temp/Humidity Variables
char humidity[10];
char celsius[10];
char fahrenheit[10];
char kelvin[10];
char dewpoint[10];
char dewpointslow[10];

//Cloud Interface Variables
WiFiClient client;
char cloudServer[] = "maker.ifttt.com";    // name address for Google (using DNS)
String IFTTT_KEY = "bt7W8RzaiUnaOwE55XTibk";
String IFTTT_EVENT = "Green GOLEM"; // IFTTT Maker Event Name here
int trigger;


void setup() {
    display_init();
    clear_display();
    Serial.begin(115200);      // initialize serial communication
    
    //Set up RTC
    myRTC.begin();
    myRTC.setTimeZone(tz_PST);

    // Date and time are local
    //
    // __DATE__ and __TIME__ are populated by Xcode during compilation
    String stampDateTime = String(__DATE__ " " __TIME__);
    // otherwise set
    // String stampDateTime = "Aug  3 2015 15:34:16";
    Serial.print("*** Date and time stamp = ");
    Serial.println(stampDateTime);
    if (convertString2DateTime(stampDateTime, "%b %d %Y %H:%M:%S", myEpochRTC))
    {
        myRTC.setLocalTime(myEpochRTC);
    }
    else
    {
        Serial.println("Error: wrong format.");
        // Fri, 31 Jul 2015 20:41:48 GMT
        myEpochRTC = 1438375308;
        Serial.print("Using default local time = ");
        Serial.println(stringDateTime(myEpochRTC));
        // Set time to RTC, only once
        myRTC.setLocalTime(myEpochRTC);
    }
    Serial.print("Set RTC = ");
    Serial.println(myEpochRTC, DEC);
    myEpochRTC = 0;
    
    //Init Schedule
    init_sched();
    
    /*              TODO                
    //Init PWM
    // Configure GPIO
    P7DIR |= BIT0;                            // P7.6~7 set TA1.1~2
    P7SEL0 |= BIT0;                           
    // Initialize LFXT1
    PJSEL0 |= BIT0 | BIT1;                 // Select for LFXT ports
    CSKEY = 0x695A;                        // Unlock CS module for register access
    CSCTL2 |= LFXT_EN;
    // Loop until XT1, XT2 & DCO fault flag is cleared
    do
    {
        // Clear XT2,XT1,DCO fault flags
       CSCLRIFG |= CLR_DCORIFG | CLR_HFXTIFG | CLR_LFXTIFG;
       SYSCTL_NMI_CTLSTAT &= ~ SYSCTL_NMI_CTLSTAT_CS_SRC;
    } while (SYSCTL_NMI_CTLSTAT & SYSCTL_NMI_CTLSTAT_CS_FLG);              // Test oscillator fault flag
    CSKEY = 0;                              // Lock CS module from unintended accesses
    // Setup Timer1_A
    TA1CCR0 = 10000-1;                                  // PWM Period
    TA1CCTL1 = OUTMOD_7;                              // CCR1 reset/set
    TA1CCR1 = 5000;                                     // CCR1 PWM duty cycle
    TA1CTL = TASSEL_1 | MC_1 | TACLR;                 // ACLK, up mode, clear TAR
    */
    
    
    /*Set Red LED as output for testing*/
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    
    //pin 2.6(40) interrupt for temp and humanidity sensor
    //pin 2.7(39) 2.4(38) for door/window interrupt
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN4);

    //Door/Window Sensor
    //attachInterrupt(40, gpio_isr, FALLING);
    //attachInterrupt(38, gpio_isr, FALLING);
    
    //WIFI connection initialization
    displayString("Connecting to: \n");
    clear_display();
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
    clear_display();
    displayString("Connected!");
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
    
    //Check Schedule and update
    int action = checkSchedule(myEpochRTC);
    if (action != -1)
    {
        
    }
    
    
    //Poll the Sensors
    int storage;
    if(MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN7)&&!lastSmoke){
        clear_display();
        displayString("Smoke Alarm!!!");
        smoke = true;
        storage = Scounter % 10;
		smokeLog[Scounter % 10] = myEpochRTC;
		Scounter++;
    }
    lastSmoke = MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN7);
    
    if(!MAP_GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4)&&lastBreach){
        clear_display();
        displayString("Door Breached!!!");
        breach = true;
    	storage = Dcounter % 10;
    	doorLog[(Dcounter % 10)] = myEpochRTC;
    	Dcounter++;
    }
    lastBreach = MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN4);
    
    //Poll the Temp/Humidity
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
    
    sprintf(humidity, "%.2f", DHT11.getHumidity());
    sprintf(celsius, "%.2f", DHT11.getCelsius());
    sprintf(fahrenheit, "%.2f", DHT11.getFahrenheit());
    sprintf(kelvin, "%.2f", DHT11.getKelvin());
    sprintf(dewpoint, "%.2f", DHT11.getDewPoint());
    
    //Update RTC
    myEpochRTC = myRTC.getTime();
    Serial.print("Get RTC  = ");
    Serial.println(myEpochRTC);
    convertEpoch2Structure(myEpochRTC, myStructureRTC);
    Serial.print("RTC = \t");
    Serial.print(stringDateTime(myEpochRTC));
    Serial.print("\r");
    convertEpoch2Structure(myEpochRTC, myStructureRTC);
    Serial.println(stringFormatDateTime("\nIt's now %I:%M %p.", myStructureRTC));
    Serial.println();
    
    //Handle Cloud Transmissions
    if (breach){
        trigger = 1;    
    }
    if(trigger == 1){
        ifttt_trigger(IFTTT_KEY, IFTTT_EVENT); 
        Serial.println("Push the button!");
        trigger = 0;
    }
    delay(1000);
}

//
//function to check if one array ends with another array
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

void handleURLRequest(char* buffer) {
    url_parse(buffer);
    
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
    	smoke = true;
    }
    if (endsWith(buffer, "GET /AlarmSmoke/L")) {
    	smoke = false;
    }
    if (endsWith(buffer, "GET /AlarmBroker/H")) {
        breach = true;
    }
    if (endsWith(buffer, "GET /AlarmBroker/L")) {
    	breach = false;
    }
    if (endsWith(buffer, "GET /AlarmTemp/H")) {
    	// Fifth Remote Control digital pin, high
    }
    if (endsWith(buffer, "GET /AlarmTemp/L")) {
    	// Fifth Remote Control digital pin, low
    }
    
    //Link to Switch handler if status changed
    if (oldSwitch1!=newSwitch1 ||
        oldSwitch2!=newSwitch2 ||
        oldSwitch3!=newSwitch3 ||
        oldSwitch4!=newSwitch4 ||
        oldSwitch5!=newSwitch5 ){
        updateSwitches();
    }
}

/* GPIO ISR */
void gpio_isr(void)
{
    storeLog();
}

/* GPIO ISR for Temperature and Humanity Sensor */
void dht11_wrapper() {
    DHT11.isrCallback();
}

/*              TODO                    */
//finish customizing me
String ifttt_trigger(String KEY, String EVENT) {
      String name = "";
      // close any connection before send a new request.
      // This will free the socket on the WiFi shield
      client.stop();
    
      // if there's a successful connection:
      if (client.connect(cloudServer, 80)) {
    
    // This is optional. You can send additional data to IFTTT along with your HTTP POST that triggers the action
    String PostData = "{\"value1\" : \"testValue\", \"value2\" : \"Hello\", \"value3\" : \"World!\" }";
    
    Serial.println("connected to server... Getting name...");
    // send the HTTP PUT request:
    String request = "POST /trigger/";
    request += EVENT;
    request += "/with/key/";
    request += KEY;
    request += " HTTP/1.1";
    
    Serial.println(request);    
    client.println(request);
    client.println("Host: maker.ifttt.com");
    client.println("User-Agent: Energia/1.1");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    client.println();

  }
  else {
    // if you couldn't make a connection:
    Serial.println("Connection failed");
    return "FAIL"; // rest API failed...
  }
  
  // Capture response from the server. (10 second timeout)
  long timeOut = 4000;
  long lastTime = millis();
  
  while((millis()-lastTime) < timeOut){  // Wait for incoming response from server
    while (client.available()) {         // Characters incoming from the server
      char c = client.read();            // Read characters
      Serial.write(c);
    }
    
  }
  Serial.println();
  Serial.println("Request Complete!");
  //return name; // Return the complete name received from server
  return "SUCCESS";
}

void sendRequest(){
  if(trigger == 0){
    trigger = 1;
  }
}


//store algorithm 
//type = 0 -> doorLog
//type = 1 -> smokeLog
void storeLog()
{
    int storage;
    if(MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN7)){
        clear_display();
        displayString("Smoke Alarm!!!");
        smoke = true;
        storage = Scounter % 10;
		smokeLog[storage] = myEpochRTC;
		Scounter++;
    }
    
    if(MAP_GPIO_getInputPinValue (GPIO_PORT_P2, GPIO_PIN4)){
        clear_display();
        displayString("Door Breached!!!");
        breach = true;
    	storage = Dcounter % 10;
    	doorLog[storage] = myEpochRTC;
    	Dcounter++;
    }
}


//fetch algorithm
//type = 0 -> doorLog
//type = 1 -> smokeLog
void updateLog(int type)
{
    int storage;
	if (type == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			storage = (Dcounter-i) % 10;
			convertEpoch2Structure(doorLog[storage], temp);
			client.println("Door Open ");
			client.println(i);
			client.println(": ");
			client.println(temp.tm_hour);
			client.println(":");
			client.println(temp.tm_min);
			client.println(":");
			client.println(temp.tm_sec);
			client.println(" ");
			client.println(temp.tm_mon);
			client.println("/");
			client.println((temp.tm_mday+1));
			client.println("/");
			client.println((temp.tm_year+1900));
			client.println("<br>");
		}
	}
	if (type == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			storage = (Scounter-i) % 10;
			convertEpoch2Structure(smokeLog[storage], temp);
			client.println("Smoke Alarm ");
			client.println(i);
			client.println(": ");
			client.println(temp.tm_hour);
			client.println(":");
			client.println(temp.tm_min);
			client.println(":");
			client.println(temp.tm_sec);
			client.println(" ");
			client.println(temp.tm_mon);
			client.println("/");
			client.println((temp.tm_mday+1));
			client.println("/");
			client.println((temp.tm_year+1900));
			client.println("<br>");
		}
	}
}
