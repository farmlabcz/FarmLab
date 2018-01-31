/*
  FLNRF24.h - Library for NRF24l01 wireless module. Designed for use in FarmLab project.
  Created by Martin Holcr, December, 2017.
  Licenced under GNU GPL 3.0 - free software
  You can redistribute it and/or modify it under the terms of the GNU 
  General Public License version 3 as published by the Free Software Foundation.
*/
#include <RF24.h>
#include "LowPower.h" 

#define PAYLOAD_SIZE 7

class FLNRF24 : public RF24
{
  public:
    FLNRF24(uint8_t CE, uint8_t CS) : RF24(CE, CS){}
    void init();
    bool checkRadio();
    //bool processData();
    // Variables for PACKET and specific parts of it
    byte PACKET[7];
    byte destinationID;
    byte senderID;
    byte sensorID;
    byte dataType;
    byte dataPriority;
    byte data;
    void sleep();    
    void wake();
    bool sendPacket();
    void makePacket(byte _destinationID, byte _senderID, byte _sensorID, byte _dataType, byte _dataPriority, byte _data );
  private:
    //Specify the reading pipes
    const uint64_t _readingPipes[5] = { 0xFFFFFF10, 0xFFFFFF11, 0xFFFFFF12, 0xFFFFFF13, 0xFFFFFF14 };
    //specify the writing pipes
    const uint64_t _writingPipes[5] = { 0xFFFFFF00, 0xFFFFFF01, 0xFFFFFF02, 0xFFFFFF03, 0xFFFFFF04 };
    bool _nrf24Up;
};

void FLNRF24::init() 
{
  begin();
  // 4 values to set for long range distance radio, set to MAX power for max long range
  // Max power 
  setPALevel( RF24_PA_MAX ) ; 
  // Min speed
  setDataRate( RF24_250KBPS ) ;
  // 8 bits CRC
  setCRCLength( RF24_CRC_8 ) ;
  // optionally, increase the delay between retries & # of retries
  setRetries(15,5);
  // optionally, reduce the payload size.  seems to
  // improve reliability
  setPayloadSize(PAYLOAD_SIZE);
  
  setChannel(120);
  
  openWritingPipe(_writingPipes[0]);
  openReadingPipe(1,_readingPipes[0]);
  openReadingPipe(2,_readingPipes[1]);
  openReadingPipe(3,_readingPipes[2]);
  openReadingPipe(4,_readingPipes[3]);
  openReadingPipe(5,_readingPipes[4]);  
  
  startListening();
  Serial.println("Initial settings done.");
  _nrf24Up = true;
}


bool FLNRF24::checkRadio()
{ 
  if ( available() )
  {
    // Dump the payloads until we've gotten everything
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = read( PACKET, PAYLOAD_SIZE );
      destinationID = PACKET [0];
      senderID = PACKET [1];
      sensorID = PACKET [2];
      dataType = PACKET [3];
      dataPriority = PACKET [4];
      data = PACKET[5];
      return true;
      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
    }
  }
}


void FLNRF24::sleep()
{ 
  stopListening();
  powerDown();
  _nrf24Up = false;
}


void FLNRF24::wake()
{
  powerUp();
  delay(10);
  startListening();
  _nrf24Up = true;
}

bool FLNRF24::sendPacket()
{

  //Serial.println("Going to send data via NRF24.");
  bool ok = false;
  unsigned long giveUpTime = 0;
  giveUpTime = millis() + 30000;
  stopListening();
  while (!ok && millis() < giveUpTime)
  {
    ok = write(&PACKET,PAYLOAD_SIZE);
    //Serial.println("T");
    //Serial.println(ok);
    //delay(10);
    //LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
    //giveUpTime-=120;
  }
  startListening();
  delay(10);
  if (ok)
  {
    //Serial.println("OK OK");
    return true;
  } else {
    //Serial.println("FALSE");
    return false;
  }
}


void FLNRF24::makePacket(byte _destinationID, byte _senderID, byte _sensorID, byte _dataType, byte _dataPriority, byte _data )
{
  destinationID = _destinationID;
  senderID = _senderID;
  sensorID = _sensorID;
  dataType = _dataType;
  dataPriority = _dataPriority;
  data = _data;
  PACKET[0] = destinationID;
  PACKET[1] = senderID;
  PACKET[2] = sensorID;
  PACKET[3] = dataType;
  PACKET[4] = dataPriority;
  PACKET[5] = data;
}