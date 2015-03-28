///////////////////////////
///////////////////////////
///////////////////////////

/*

  This sketch demonstrates all the different options
  available when using an RFm69 radio tranceiver.
  
  See the "wiring_rfm69.png" for how to hookup the circuit.
  
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

///////////////////////////
///////////////////////////
///////////////////////////

void setup() {
  Serial.begin(9600);
  
  radio.initialize(RF69_915MHZ, 1, 1);
  
  radio.setPowerLevel(31); // min is 0, max is 31
  // radio.setHighPower(); // only if using the HW series, turn on built in amplifiers
  
  radio.promiscuous(); // capture all packets on this network (no ID filtering)
  radio.promiscuous(false); // turn promiscuous mode OFF
  
  radio.setFrequency(915000000); // change the radio's frequency (frequency hopping anyone?)
  unsigned long currentFrequency = radio.getFrequency();
  
  char encryptKey[] = "Secret Key";
  radio.encrypt(encryptKey); // encrypt your packets with a shared key
  
  radio.sleep(); // put the radio to sleep when inactive to save power
  // the radio will wake up the next time you call one of it's functions
}

///////////////////////////
///////////////////////////
///////////////////////////

void loop() {
}

///////////////////////////
///////////////////////////
///////////////////////////
