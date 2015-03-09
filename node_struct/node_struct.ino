///////////////////////////
///////////////////////////
///////////////////////////

#include <RFM69.h>
#include <SPI.h> // the RFM69 library uses SPI

RFM69 radio;

// our pre-defined packet structure
// this struct is shared between all nodes
typedef struct {
  int sensorOne;
  int sensorTwo;
} Packet;

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);
  
  // setup the radio
  radio.initialize(RF69_915MHZ, 1, 1);
  
  Serial.println("RADIO INITIALIZED\n\n");
  Serial.println("Enter '1' to broadcast a Packet");
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
  
  if(Serial.available()>0){
    if(Serial.read()=='1'){
  
      // create new instance of our Packet struct
      Packet packet;
      packet.sensorOne = analogRead(A0); // write values from the analog pins
      packet.sensorTwo = analogRead(A1);
      
      // broadcasting this packet, so all ID's on this network can hear it
      radio.send(255,  &packet, sizeof(packet)); // notice the & next to packet
      Serial.println("Sent");
    }
  }
  
  // ALWAYS check to see if we've received a message
  if (radio.receiveDone()) {
          
    // if the received message is the same size as our pre-defined Packet
    // then assume that it is actually one of our Packets
    if(radio.DATALEN == sizeof(Packet)) {
    
      // convert the radio's raw byte array to our pre-defined Packet struct
      Packet newPacket = *(Packet*)radio.DATA;
      Serial.print("[");
      Serial.print(radio.SENDERID);
      Serial.print("] sensorOne = ");
      Serial.print(newPacket.sensorOne);
      Serial.print("\tsensorTwo = ");
      Serial.println(newPacket.sensorTwo);
    }
  }
}

///////////////////////////
///////////////////////////
///////////////////////////
