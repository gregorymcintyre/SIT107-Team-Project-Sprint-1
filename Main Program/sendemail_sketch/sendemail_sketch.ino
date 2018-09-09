#include <Bridge.h>
#include <TembooYunShield.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Bridge.begin();
  Console.begin();
}

void loop() {
  if (calls <= maxCalls) {
    Console.println("Running SendEmail - Run #" + String(calls++));
    
    TembooYunShieldChoreo SendEmailChoreo;

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    SendEmailChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);
    
    // Set Choreo inputs
    SendEmailChoreo.addInput("FromAddress", "hrandemail@gmail.com");
    SendEmailChoreo.addInput("Username", "hrandemail@gmail.com");
    SendEmailChoreo.addInput("Subject", "Alert");
    SendEmailChoreo.addInput("ToAddress", "hrandemail@gmail.com");
    SendEmailChoreo.addInput("MessageBody", "Alert");
    SendEmailChoreo.addInput("Password", "ghqqrdvqokwoohaw");
    
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    
    // Run the Choreo; when results are available, print them to Console
    SendEmailChoreo.run();
    
    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Console.print(c);
    }
    SendEmailChoreo.close();
  }

  Console.println("Waiting...");
  delay(30000); // wait 30 seconds between SendEmail calls
}