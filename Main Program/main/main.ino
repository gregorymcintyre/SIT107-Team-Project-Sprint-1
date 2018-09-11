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

//web server settings
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 20, 177);
EthernetServer server(80);

//HR Variables
int Signal;
int PuleseSensorPin = 0;
int Threshold  = 500;


//HR Global Variables
bool HRIrregular = false;
int HeartRate = 0;
int Counter = 0; //test counter

void setup() {
  Serial.begin(9600);
  
  //webserver setup
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  //Heart Rate Setup
  pinMode(LED_BUILTIN, OUTPUT); //testing
  
}


void loop() {
  readData(); //sense
  assessData(); //think
  //WriteHTML(); //act
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
            client.print(HeartRate);
            client.println("<br />");
            
            if (HRIrregular){
              client.print("Heart Rate is IRREGULAR");
            }
            else{
              client.print("Heart Rate is Normal");
            }
            client.println("<br />");

            //testing counter
            client.println(Signal);
            client.println("<br />");
            
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

void assessData(){
  if (HeartRate < 30){
    HRIrregular = true;
  }
  else{
    HRIrregular = false;
  }
}//think


void readData(){
  /* readData
   *  
   *  Tiffany Gray
   *    
   *  read analog data from the 
   * 
   */

  Signal = analogRead(PuleseSensorPin);
  Serial.println(Signal);
  delay(10);

  if(Signal < Threshold){
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println("beat");
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }

   
}//sense



