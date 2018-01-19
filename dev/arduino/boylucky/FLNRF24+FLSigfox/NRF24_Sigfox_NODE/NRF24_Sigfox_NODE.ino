/*
  Sketch for basic node of FarmLab project in NRF24 + Sigfox LPWAN network.
  Designed for use in FarmLab project.
  Created by Martin Holcr, December, 2017.
  Licenced under GNU GPL 3.0 - free software
  You can redistribute it and/or modify it under the terms of the GNU 
  General Public License version 3 as published by the Free Software Foundation.
*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "LowPower.h"
//#include "printf.h" // - only for testing, comment after testing done

// Libraries used for connector type: NRF - Sigfox LPWAN
#include "FLSigfox.h"
#include "FLNRF24.h"

// Sensor
#include "DHT.h"
DHT dht;
byte humidity;
byte temperature;
#define DHT11PIN 6

// Settings
#define MY_NODE_ID 1
#define NODE_SEND_INTERVAL 1 // Interval to send data from sensors in minutes

//unsigned long nextSleep = 5000;
unsigned long sleepCycleCounter = 0;
unsigned long nextNodeSend = 0;

bool readyForSleep = false;

// Initiate FLNRF24 object with ce, csn pin
FLNRF24 nrf24(9,10);

void dhtRead(){
  digitalWrite(5,HIGH);
  delay(dht.getMinimumSamplingPeriod());

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
/*
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print((byte)humidity, 1);
  Serial.print("\t\t");
  Serial.print((byte)temperature, 1);
*/  
  digitalWrite(5,LOW); 
  delay(10);
}


void setup() {
  Serial.begin(9600);
  nrf24.init();
  //nrf24.makePacket(2,MY_NODE_ID,1,1,(byte) millis());
  //nrf24.sendPacket();
  // DHT setings
  pinMode(5, OUTPUT);
  pinMode(DHT11PIN, OUTPUT);
  dht.setup(6); // Data on pin 6
  //Serial.println("Initial phase done.");
}

void loop() {
  // Check if any data received on the NRF24 radio

  if (nrf24.checkRadio())
  {
    //Serial.print("Data received on NRF24 radio, from NODE: ");
    //Serial.println(nrf24.senderID);
  }

// Check if time to send data via NRF24
//Serial.println(nextNodeSend);
//Serial.println(millis());
   if (millis() > nextNodeSend)
   { 
     dhtRead();
     nrf24.makePacket(0,MY_NODE_ID,1,0,0,humidity);
     if (nrf24.sendPacket())
     {
       //Serial.println("Data send via NRF24 successfuly.");
     } else {
       //Serial.println("Failed to send data via NRF24.");
     }

     nrf24.makePacket(0,MY_NODE_ID,2,0,0,temperature);
     if (nrf24.sendPacket())
     {
       //Serial.println("Data send via NRF24 successfuly.");
     } else {
       //Serial.println("Failed to send data via NRF24.");
     }

     
     nextNodeSend+= (NODE_SEND_INTERVAL * 30000);
     readyForSleep = true;
   }
   
   if (readyForSleep)
   {
     // Enter power down state of NRF24
     //Serial.println("Chip + NRF24 going to sleep now.");
     nrf24.sleep();
     //delay(2000);
      
     while (millis() < nextNodeSend)
     { 
       //Serial.println(nextNodeSend);
       //Serial.println(millis());
       //delay(2000);
       // Enter power down state for 8 s with ADC and BOD module disabled
       LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
       if (nextNodeSend < 8001) {nextNodeSend = 8001;}
       nextNodeSend-=8000; // If sleep duration change then this value also have to be changed according to sleep duration milliseconds
     }
     // Power Up NRF24 module
     nrf24.wake();
     //Serial.println("Chip + NRF24 wake up now.");
     readyForSleep = false;
   }

  
}
