/*main.ino
 * 
 * Tiffany Grey and Greg McIntyre
 * 
 * uses a web server to show network connectivity and display status' in future development these messages with be transmitted by email/sms
 * 
 * 
 */

//Web Server Libraries
#include <SPI.h> 
#include <Ethernet.h>
#include <SimpleTimer.h>
#include "SD.h"
#include <Wire.h>
#include "RTClib.h"

//RTC Setting
RTC_DS1307 RTC;
const int chipSelect = 10;
File logfile;
DateTime now;

//web server settings
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 20, 177);
EthernetServer server(80);

//HR Variables
int Signal;
int PuleseSensorPin = 0;
int UpperThreshold  = 525;
int LowerThreshold  = 505;
bool Beat = true;
int HRcount = 0;


//HR Analysis Variables
bool HRIrregular;
int HeartRate = 0;
//int Counter = 0; //test counter
SimpleTimer timer;
int BPM = 0;


void setup() {
  Serial.begin(9600);
  
  //webserver setup
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("System started. Server is at ");
  Serial.println(Ethernet.localIP());

  //Heart Rate Setup
  pinMode(LED_BUILTIN, OUTPUT); //testing

  initSDcard();
  createFile();
  initRTC();

  //timer setup
  timer.setInterval(5000, BPMCalc);
  
}


void loop() {
  
  timer.run();  //start interval timer/think
  //WriteHTML(); //load web server/act
  readData(); //sense

}

void WriteHTML (){
  /* WriteHTML 
   *  
   *  Greg Mcintyre
   *  
   *  writes a html webserver with an output of the information. Displays status information in console and display information in webserver. web server is intentionally kept simeple for future development into email/sms
   *  
   *  can be accessed at 'IPAddress' variable
   * 
   */
   
  //Web Server Output
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

            client.print("Current heart rate is ");
            client.print(BPM);
            client.print(" BPM");
            client.println("<br />");
            
            if (HRIrregular == true){
              client.print("Heart Rate is IRREGULAR");
            } else {
              client.print("Heart Rate is Normal");
            }
            client.println("<br />");

            //testing counter
            //client.println(Signal); //testing
            //client.println("<br />");
            
          //}
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }


}//act

void readData(){
  /* readData
   *  
   *  Tiffany Gray
   *    
   *  read analog data from the pulse sensor
   *  adds to HRCount variable uses a boolean to identify if 'beat' has been counted
   * 
   */

  Signal = analogRead(PuleseSensorPin);
  
  
  //Serial.println(Signal);

  if (Signal < 550 && Signal > 450){  //trying to filter

    if(Signal >= UpperThreshold && Beat == false){
      //if(Signal >= Threshold){
          Beat = true;
          HRcount++;
          //Serial.println("Beat");
          //Serial.println(HRcount);
    }

    if (Signal < LowerThreshold){
      Beat = false;
      //Serial.println("Reset");
    }
  }

   
}//sense


void BPMCalc(){
/*BPM Calc
 * 
 * Greg McIntyre
 * 
 * Sample is based on the 5 second refresh of the web server for consistancy not for function
 * this updates the BPM and HRIrregular variables
 * 
 * 
 */

  
  BPM = HRcount*12;
  HRcount = 0;
  //Serial.println(BPM);

  if ((BPM >= 40) && (BPM <=190)){
    HRIrregular = false;
    //Serial.println("*****************************Normal");
  } else {
    HRIrregular = true;
    //Serial.println("*****************************Irregular");
  }

  logtoFile();
}

void initSDcard()
{
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}

void createFile()
{
  //file name must be in 8.3 format (name length at most 8 characters, follwed by a '.' and then a three character extension.
  char filename[] = "MLOG00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i / 10 + '0';
    filename[5] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);
}

void initRTC()
{
  Wire.begin();
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL

  }
}

void error(char const *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while (1);
}

void logtoFile()
{
    now = RTC.now();
    logfile.print(now.year(), DEC);
    logfile.print("/");
    logfile.print(now.month(), DEC);
    logfile.print("/");
    logfile.print(now.day(), DEC);
    logfile.print(" ");
    logfile.print(now.hour(), DEC);
    logfile.print(":");
    logfile.print(now.minute(), DEC);
    logfile.print(":");
    logfile.print(now.second(), DEC); //logs date stamp

    logfile.print(", ");
    logfile.print(BPM);
    logfile.println(" bpm");  //logs bpm

    logfile.flush();

    //Serial.print("Logged "); //Testing
    //Serial.print(BPM);
    //Serial.println(" bpm");  //logs bpm
    
    
}



