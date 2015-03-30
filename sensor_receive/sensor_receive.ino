///////////////////////////
///////////////////////////
///////////////////////////

/*

  This sketch demonstrates a simple wireless hub
  listening for sensor packets from other nodes on this network.
  To make parsing packets easier, data is sent using the C++ struct.
  This allows both sender and receiver to read and write to the packet
  using the familiar object-dot-variable syntax.
  
  See the "wiring_rfm69.png" for how to hookup the circuit.
  
  To complete the example, run the "sensor_send.ino" sketch,
  or the "sensor_send_lowpower.ino" sketch
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
#include <SPI.h> // the RFM69 library uses SPI

RFM69 radio;

#define myFrequency RF69_915MHZ // or RF69_433MHZ (check your radio)
int myNetwork = 123; // radios must share the same network (0-255)
int myID = 0; // radios should be given unique ID's (0-254, 255 = BROADCAST)

// our pre-defined packet structure
// this struct must be shared between all nodes
typedef struct {
  int sensor0;
  int sensor1;
  int sensor2;
} Packet;

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);

  // setup the radio
  radio.initialize(myFrequency, myID, myNetwork);
    
  Serial.println("\nRADIO INITIALIZED\n");
  Serial.println("Listening for sensor nodes...");
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
    
  // always check to see if we've received a message
  if (radio.receiveDone()) {
              
    // if the received message is the same size as our pre-defined Packet struct
    // then assume that it is actually one of our Packets
    if(radio.DATALEN == sizeof(Packet)) {  
      
      // convert the radio's raw byte array to our pre-defined Packet struct
      Packet newPacket = *(Packet*)radio.DATA;
      int senderID = radio.SENDERID;
      
      // if requested, acknowledge that the packet was received
      if (radio.ACKRequested()){
        radio.sendACK();
      }
      
      Serial.print("(");
      Serial.print(senderID);
      Serial.print(")\t");
      Serial.print(newPacket.sensor0);
      Serial.print("\t");
      Serial.print(newPacket.sensor1);
      Serial.print("\t");
      Serial.println(newPacket.sensor2);
    }
    else {
      Serial.println("got unknown packet!");
    }
  }
}

///////////////////////////
///////////////////////////
///////////////////////////
