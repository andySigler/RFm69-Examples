///////////////////////////
///////////////////////////
///////////////////////////

/*

  This sketch demonstrates a simple wireless sensor node,
  sending packets to the receiver on this network.
  To make parsing packets easier, data is sent using the C++ struct.
  This allows both sender and receiver to read and write to the packet
  using the familiar object-dot-variable syntax.
  
  See the "wiring_rfm69.png" for how to hookup the circuit.
  
  To complete the example, run the "sensor_receive.ino" sketch
  on another Arduino with an RFm69 connected
  
  Be sure you have downloaded and installed the library used here:
  
    RFm69 Library: https://github.com/lowpowerlab/rfm69
    Narcoleptic Library: https://code.google.com/p/narcoleptic/downloads/list

*/

///////////////////////////
///////////////////////////
///////////////////////////

#include <RFM69.h>
#include <SPI.h> // the RFM69 library uses SPI
#include <Narcoleptic.h> // the lowpower library for putting the CPU to sleep

RFM69 radio;

#define myFrequency RF69_915MHZ // or RF69_433MHZ (check your radio)
int myNetwork = 123; // radios must share the same network (0-255)
int myID = 1; // radios should be given unique ID's (0-254, 255 = BROADCAST)

int hubID = 0; // the receiver for all sensor nodes in this example

int secondsDelay = 5; // the number of seconds this node will sleep between sensor readings

// instead of sending a string, we can send a struct
// this struct must be shared between all nodes
typedef struct {
  int sensorReading; // value from analogRead()
  unsigned long aliveTime; // how long this node's been running, in milliseconds
} Packet;

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);
  
  // setup the radio
  radio.initialize(myFrequency, myNetwork, myID);
  
  radio.setPowerLevel(10); // min is 0, max is 31
  
  Serial.println("\nRADIO INITIALIZED\n");
  Serial.print("Sending sensor values every ");
  Serial.print(secondsDelay);
  Serial.println(" seconds");
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
  
  // first put the radio and the CPU to sleep, saving precious battery life
  // the radio will not receive data while sleeping
  
  radio.sleep(); // radio will wake back up when we send data
  
  // put microcontroller to sleep for a number of milliseconds
  Narcoleptic.delay(secondsDelay * 1000); // convert seconds to milliseconds
  
  // create new instance of our Packet struct
  Packet packet;
  packet.sensorReading = analogRead(A0); // read values from the analog pins
  packet.aliveTime = millis(); // how long has this microcontroller been on?
  
  int numberOfRetries = 5;
  
  // send reliable packet to the hub
  // notice the & next to packet when sending a struct
  boolean gotACK =  radio.sendWithRetry(hubID,  &packet, sizeof(packet), numberOfRetries);
  
  if(gotACK) {
    Serial.println("acknowledged :)");
  }
  else {
    Serial.println("failed getting acknowledgement :(");
  }
}

///////////////////////////
///////////////////////////
///////////////////////////
