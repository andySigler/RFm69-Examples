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
  
  To complete the example, run the "sensor_send.ino" sketch
  on another Arduino with an RFm69 connected
  
  Be sure you have downloaded and installed the library used here:
  
    RFm69 Library: https://github.com/lowpowerlab/rfm69

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
  int sensorReading;
  unsigned long aliveTime;
} Packet;

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);

  // setup the radio
  radio.initialize(myFrequency, myNetwork, myID);
    
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
      
      // read the values from the data struct
      int val = newPacket.sensorReading;
      unsigned long time = newPacket.aliveTime;
      
      Serial.print("[");
      Serial.print(radio.SENDERID);
      Serial.print("]\tsensorReading = ");
      Serial.print(val);
      Serial.print("\taliveTime = ");
      Serial.println(time);
    }
  }
}

///////////////////////////
///////////////////////////
///////////////////////////
