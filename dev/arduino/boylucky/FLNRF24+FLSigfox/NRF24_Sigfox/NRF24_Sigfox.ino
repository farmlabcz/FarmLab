/*
  Sketch for NRF24 and Sigfox LPWAN module.
  Designed for use in FarmLab project.
  Created by Martin Holcr, December, 2017.
  Licenced under GNU GPL 3.0 - free software
  You can redistribute it and/or modify it under the terms of the GNU 
  General Public License version 3 as published by the Free Software Foundation.
*/

/*
  Sigfox LoRaWAN modul is connected to hardware serial rx (digital 0 pin),
  tx (digital 1 pin) of Arduino Uno or ATMEGA328P.
  NRF24L01+ modul is connected to following pins:
  
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

// Settings
#define MY_NODE_ID 0
#define SIGFOX_SEND_INTERVAL 2 // Sigfox LPWAN module connection interval in minutes
#define WAKE_DURATION 500 // Wake duration of the AVR and Radio in milliseconds
//#define SLEEP_DURATION SLEEP_8S // Sleep duration of AVR and NRF24


unsigned long nextSleep = 5000;
unsigned long sleepCycleCounter = 0;
unsigned long nextSigfoxSend = 20000;

// Initiate FLSigfox object with digital pin number where LED can be attached
// for Sigfox LoRaWAN modul status indication.
// Sigfox LoRaWAN modul must have rx and tx pins connected to hardware rx, tx pin
// of Arduino or ATMEGA328P. Hardware serial communication is used.
FLSigfox sigfox(6);

// Initiate FLNRF24 object with ce, csn pin
FLNRF24 nrf24(8,10);


void setup() {
  Serial.begin(9600);
  nrf24.init();
  sigfox.wake();
  sigfox.ready();
  //Serial.print("AT$SF=1234565269353621457892");
  //Serial.print((byte)millis());
  //Serial.write(10);
  //delay(13000);
  sigfox.sleep();
}

void loop() {
  // Check if any data received on the NRF24 radio
  if (nrf24.checkRadio())
  {

     //Serial.println(millis());
     //Serial.println(nextSigfoxSend);
    
    //Serial.println("Data received on NRF24 radio.");
    sigfox.sensorAutoStore(nrf24.sensorID,nrf24.data,nrf24.dataPriority);
/*
    Serial.print("Received payload: ");
    Serial.println(nrf24.PACKET[0]);
    Serial.println("First byte:"); 
    Serial.println(nrf24.destinationID);
    Serial.println("Second byte:"); 
    Serial.println(nrf24.senderID);
    Serial.println("Third byte:"); 
    Serial.println(nrf24.sensorID);
    Serial.println("Fourth byte:"); 
    Serial.println(nrf24.dataType);
    Serial.println("Fifth byte:"); 
    Serial.println(nrf24.data);
/*
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(6));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(0));
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(7));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(1));
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(8));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(2));
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(9));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(3));
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(10));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(4));
    
    Serial.println("Sensor ID:"); 
    Serial.println(sigfox.readStore(11));
    Serial.println("Sensor value:"); 
    Serial.println(sigfox.readStore(5));
    */
  }
   // Check if time to go to Power Down mode of NRF24 and controller

   if (millis() > nextSleep)
   { 
     // Enter power down state of NRF24
     //Serial.println("Chip + NRF24 going to sleep now.");
     nrf24.sleep();
     //delay(1000);
     // Enter power down state for 8 s with ADC and BOD module disabled
     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
     // Power Up NRF24 module
     nrf24.wake();
     nextSleep = millis() + WAKE_DURATION;
     sleepCycleCounter+= 1;
     if (nextSigfoxSend < 8001) {nextSigfoxSend = 8001;}
     nextSigfoxSend-= 8000; // If sleep duration change then this value also have to be changed according to sleep duration milliseconds
     //Serial.println("Chip + NRF24 wake up now.");
     //delay(1000);    
   }


   // Check if time to send data via Sigfox LPWAN
   if (millis() > nextSigfoxSend)
   { 
     //Serial.println("Time to send data via Sigfox.");
     nrf24.sleep();
     delay(10);
     sigfox.wake();
     if (sigfox.dataSend())
     {
       //Serial.println("Data send successfully via Sigfox.");
     } else {
       // Retry one more time to send data via Sigfox
       //sigfox.dataSend();
       //Serial.println("Failed to send data via Sigfox.");
     }

     sigfox.sleep();
     nrf24.wake();
     sigfox.dataClear();
     nextSigfoxSend+= SIGFOX_SEND_INTERVAL * 60000;

     //Serial.println(millis());
     //Serial.println(nextSigfoxSend);
   }
   
}
