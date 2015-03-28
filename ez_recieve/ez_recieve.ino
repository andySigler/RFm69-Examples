///////////////////////////
///////////////////////////
///////////////////////////

/*

  This sketch demonstrates a simple wireless hub
  listening for strings from other nodes on this network.
  
  See the "wiring_rfm69.png" for how to hookup the circuit.
  
  To complete the example, run the "ez_send.ino" sketch
  on another Arduino with an RFm69 connected
  
  Be sure you have downloaded and installed the library used here:
  
    RFm69 Library: https://github.com/lowpowerlab/rfm69

  Created 24 March 2015
  By Andy Sigler
  
*/

///////////////////////////
///////////////////////////
///////////////////////////

#include <RFM69.h>
#include <SPI.h>

RFM69 radio;

#define myFrequency RF69_915MHZ // or RF69_433MHZ (check your radio)
int myNetwork = 100; // radios must share the same network (0-255)
int myID = 1; // radios should be given unique ID's (0-254, 255 = BROADCAST)

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);
  
  // setup the radio
  radio.initialize(myFrequency, myID, myNetwork);
  
  Serial.println("RADIO INITIALIZED");
  Serial.println("Listening...");
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
  // will return true once a complete message has been received
  if (radio.receiveDone()) {
    
    Serial.print("[");
    Serial.print(radio.readRSSI()); // get the signal strength in dB
    Serial.print("] ");
    
    int senderID = radio.SENDERID; // the sending radio's ID
    Serial.print(senderID);
    Serial.print(" : ");
        
    // we'll just assume the new payload should be read as a string
    String payload = "";  
    for(int i=0;i<radio.DATALEN;i++){ // loop through all the new bytes
      char temp = radio.DATA[i]; // casting to char will convert integer to ascii
      payload += temp; // append the new char to the string
    }
    Serial.print(payload);
    
    // send an acknowledgement if the packet asks for one
    if (radio.ACKRequested()){
      radio.sendACK();
      Serial.print(" - ACK sent");
    }
    
    Serial.println();
  }
}

///////////////////////////
///////////////////////////
///////////////////////////
