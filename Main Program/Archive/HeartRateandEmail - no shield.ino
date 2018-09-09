/*HeartRatandEmail.ino
 * 
 * Tiffany Gray
 * Greg McIntyre 
 * 
 * This program is designed to; 
 * 1. rx data from the Z6352 Heart Rate Monitor
 * 2. intrepret the data and assess if emergency services or emergencey contact sould be contacted
 * 3. notify the parties as required
 * 
 * Future Developments
 *  - sms intergration
 * 
 *
 *
 */

int dataPin = 10;       //data pin from HR Monitor
bool detected = false;  //to avoid multiple sends
char data_rx;           //data

void setup() {
  //general setup
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  //Email output setup
  pinMode(dataPin, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Intialisation Complete");

}

void loop() {

  
  //Email output loop
    /*if (digitalRead(dataPin)==HIGH){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
  } //switch proof of concpet */
  
  if(digitalRead(dataPin) == LOW && !detected)
  if(digitalRead(dataPin) == LOW)
  {
    Serial.println("1");
    detected = true;
      
  }else if(digitalRead(dataPin)== HIGH && detected)
  //}else if(digitalRead(dataPin)== HIGH)
  {
    detected=false;
  }

  //data_rx=Serial.read();

  if (data_rx == '1'){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
  }



    
}

