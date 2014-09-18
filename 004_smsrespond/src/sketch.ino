/*
  SMS Responder
*/

// Include the GSM library
#include <GSM.h>

#define PINNUMBER "4221"

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];  

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("SMS Messages Sender");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  
  Serial.println("GSM initialized");
}

void loop()
{

  char c;
  char txtMsg[200];
  int count = 0;
  
  // If there are any SMSs available()  
  if (sms.available())
  {
    Serial.println("Message received from:");
    
    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    // An example of message disposal    
    // Any messages starting with # should be discarded
    // if(sms.peek()=='#')
    // {
    //   Serial.println("Discarded SMS");
    //   sms.flush();
    // }
    
    // Clear Message
    memset(&txtMsg[0], 0, sizeof(txtMsg));
    // Read Message
    while(c=sms.read()) {
      Serial.print(c);
      txtMsg[count++]  = c;
    }
      
    Serial.println("\nEND OF MESSAGE");
    
    // send the message
    sms.beginSMS(senderNumber);
    Serial.println(txtMsg);
    sms.print(txtMsg);
    sms.endSMS(); 
    Serial.println("\nCOMPLETE!\n");

    // Delete message from modem memory
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }
 delay(1000);

}

/*
  Read input serial
 */
int readSerial(char result[])
{
  int i = 0;
  while(1)
  {
    while (Serial.available() > 0)
    {
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if(inChar!='\r')
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}