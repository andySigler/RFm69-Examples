///////////////////////////
///////////////////////////
///////////////////////////

#include <RFM69.h>
#include <SPI.h> // the RFM69 library uses SPI

RFM69 radio;

#define myFrequency RF69_915MHZ // or RF69_433MHZ (check your radio)
int myNetwork = 100; // radios must share the same network (0-255)
int myID = 0; // radios should be given unique ID's (0-254, 255 = BROADCAST)

int receiverID = 1; // the ID of our receiver, must be on this network and frequency

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);
  
  // setup the radio
  radio.initialize(myFrequency, myID, myNetwork);
  
  Serial.println("RADIO INITIALIZED\n\n");
  Serial.println("Enter '1' to send unreliable message");
  Serial.println("Enter '2' to send reliable message");
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
    
  if(Serial.available()>0) {
    
    char input = Serial.read();
    
    byte payload[] = "Hello Wireless"; // send strings with an array of bytes
    int payloadSize = sizeof(payload); // get the size of our payload
    
    if(input=='1'){
      
      // bare-bones sending (no acknowledgement)
      // this means less latency
      // but we don't know if the message was received
      
      radio.send(receiverID, payload, payloadSize);
      Serial.println("Sent");
    }
        
    else if(input=='2') {
      
      // send a message that asks for an acknowledgment
      // checking the connection like this adds reliability
      // but will also add latency
      
      Serial.print("Sending...");
      
      int retries = 3; // maximum number of times this message will attempt to get acknowledged
      
      boolean didReceive = radio.sendWithRetry(receiverID, payload, payloadSize, retries);
    
      if(didReceive) {
        Serial.print("Acknowledged");
        Serial.print(" [");
        Serial.print(radio.readRSSI()); // get the signal strength in dB
        Serial.println("] ");
      }
      else {
        Serial.println("Failed");
      }
    }
  }
}

///////////////////////////
///////////////////////////
///////////////////////////