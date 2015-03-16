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

// our pre-defined packet structure
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
  
  radio.sleep(); // radio will wake back up when we send data
  Narcoleptic.delay(secondsDelay * 1000); // convert seconds to milliseconds for delay()
  
  // create new instance of our Packet struct
  Packet packet;
  packet.sensorReading = analogRead(A0); // read values from the analog pins
  packet.aliveTime = millis();
  
  int numberOfRetries = 3;
  
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
