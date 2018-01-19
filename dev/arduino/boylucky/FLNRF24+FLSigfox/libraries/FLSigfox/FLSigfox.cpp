/*
  FLSigfox.h - Library for LPWAN Sigfox module. Designed for use in FarmLab project.
  Created by Martin Holcr, December, 2017.
  Licenced under GNU GPL 3.0 - free software
  You can redistribute it and/or modify it under the terms of the GNU 
  General Public License version 3 as published by the Free Software Foundation.
*/

#include "Arduino.h"
#include "FLSigfox.h"
#include <SoftwareSerial.h>


FLSigfox::FLSigfox(byte rx,byte tx, int baudRate) 
{
  mySerial = new SoftwareSerial(rx,tx);
  mySerial->begin(baudRate);
}

void FLSigfox::sleep()
{
  mySerial->print('\r');
  mySerial->print("AT$P=1");
  mySerial->print('\r');
  _sigfoxUp=false;
  delay(10);
}

void FLSigfox::wake()
{
  mySerial->print('\n');
  mySerial->print('\r');
  _sigfoxUp=true;
  delay(500);
}

void FLSigfox::insideSensorsRead()
{
  String inputString = "";      // string to hold incomming data
  while (mySerial->available()) {  // // read incomming data if any to clear possible previous messages from Sigfox. Clear the Serial buffer.
    mySerial->read();
  }
  mySerial->print("AT$V?");
  mySerial->print('\r');

  // Wait until sigfox return something
  while (!mySerial->available()) {
    delay(10);
  }
  // Revrite to get also the voltage measured during last transmission
  while (mySerial->available()) {
    // get the new byte:
    char inChar = (char)mySerial->read();
    // add it to the inputString:
    inputString += inChar;
  }

  voltage = inputString.toInt();


  inputString = "";      // string to hold incomming data

  while (mySerial->available()) {  // read incomming data if any to clear possible previous messages from Sigfox. Clear the Serial buffer.
    mySerial->read();
  }
  
  mySerial->println("AT$T?");
  
  // Wait until sigfox return something
  while (!mySerial->available()) {
    delay(10);
  }
  while (mySerial->available()) {
    // get the new byte:
    char inChar = (char)mySerial->read();
    // add it to the inputString:
    inputString += inChar;
  }

  temperature = inputString.toInt();

  while (mySerial->available()) {  // read incomming data if any to clear possible other messages from Sigfox. Clear the Serial buffer.
    mySerial->read();
  }

}



bool FLSigfox::dataSend() {
  String inputString = "";         // string to hold incoming data

  // Clear possible data in the serial buffer
  while (mySerial->available()) {
    char inChar = (char)mySerial->read();
  }

  mySerial->print("AT$SF=");
  for (byte i=0; i<12; i++)
  {
  mySerial->print(toHex(_dataArray[i]));
  }
  mySerial->print('\r');

  //Sigfox.println(zprava);
  // Wait until sigfox return something
  while (!mySerial->available()) {
    delay(10);
  }

  // Then read the response  
  while (mySerial->available()) {
    // get the new byte:
    char inChar = (char)mySerial->read();
    // add it to the inputString:
    if (inChar != '\r' && inChar != '\n') {
      inputString += inChar;    
    }

  }
  Serial.println(inputString);
  
  if (inputString == "OK") {
    return true;
  } else {
    return false;
  }
}


// toHex
// - convert value to two hex characters
// - the usual Arduino functions output e.g. "0" instead of "00"
char *FLSigfox::toHex( uint8_t c ) {
  const static char *hex="0123456789ABCDEF";
  static char toHexBuf[3];
  toHexBuf[0]= hex[c>>4];
  toHexBuf[1]= hex[c & 0x0f];
  toHexBuf[2]=0;
  return toHexBuf;
}


void FLSigfox::sensorStore(byte position, byte value, byte priority, byte sensorID) {
  _dataArray[position] = value;
  _dataArray[position+6] = sensorID;
  _priorityStatus[position] = priority;
}

/*
bool FLSigfox::sensorPriority(byte sensorID, byte priority) {
  if (sensorID>50) {
    return false;
  } else {
    _priorityArray[sensorID] = priority;
    return true;
  }
}
*/

bool FLSigfox::sensorAutoStore(byte sensorID, byte value, byte priority) {
  bool positionFound = false;
  byte i = 0;
  byte futurePosition = 100;
  byte lowestPriority = 5;
  for (i=0; i<6; i++)
  {
    if (_dataArray[i+6] == sensorID)
    {
      _dataArray[i] = value;
      _dataArray[i+6] = sensorID;
      _priorityStatus[i] = priority;
      positionFound = true;
      return true;
    }
    tempMessage = 250;
  }
  i=0;
  while (!positionFound) {
    if (_dataArray[i] == 255) {
      _dataArray[i] = value;
      _dataArray[i+6] = sensorID;
      _priorityStatus[i] = priority;
      positionFound = true;
      return true;
    } else {
      if (_priorityStatus[i] < lowestPriority) {
        futurePosition = i;
        lowestPriority =  _priorityStatus[i];
      }
    }
    if (i==5) {
      if (lowestPriority == 6 || futurePosition == 100) {
        return false;
      } else {
        _dataArray[futurePosition] = value;
        _dataArray[futurePosition+6] = sensorID;
        _priorityStatus[futurePosition] = priority;
      }
    }
   i+=1;  
  }
}


void FLSigfox::dataClear() {
  for (byte i=0; i<12; i++) {
    _dataArray [i] = 255;
  }
  for (byte i=0; i<6; i++) {
    _priorityStatus[i] = 0;
  }
}

byte FLSigfox::readStore (byte position) {
  return _dataArray [position];

}