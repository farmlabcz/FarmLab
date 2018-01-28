/*
  FLSigfox.cpp - Library for LPWAN Sigfox module. Designed for use in FarmLab project.
  Created by Martin Holcr, December, 2017.
  Licenced under GNU GPL 3.0 - free software
  You can redistribute it and/or modify it under the terms of the GNU 
  General Public License version 3 as published by the Free Software Foundation.
*/
#ifndef FLSigfox_h
#define FLSigfox_h
#include <SoftwareSerial.h>

#include "Arduino.h"

#define SIGFOX_READY 1
#define SIGFOX_NOT_READY 2

class FLSigfox
{
  public:
    FLSigfox(byte msgPin); //rx, tx pin and baud rate for SoftwareSerial communication.
    void insideSensorsRead();
    void sleep();
    void wake();
    void voltageRead();
    void tempRead();
    void sensorStore(byte position, byte value, byte priority, byte sensorID);
    //bool sensorPriority(byte sensorID, byte priority);
    bool sensorAutoStore(byte sensorID, byte value, byte priority);
    byte readStore(byte position);
    void dataClear();
    bool dataSend();
    int temperature;
    int voltage;
    byte tempMessage;
    bool ready();
    void msgLed(byte message);
    //void softwareReset();
    void pinSet();
  private:
    char *toHex( uint8_t c );
    byte _dataArray[12] = {255,255,255,255,255,255,255,255,255,255,255,255};
    byte _priorityStatus[6] = {0,0,0,0,0,0};
    //byte _priorityArray[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool _sigfoxUp;
    SoftwareSerial* mySerial;
    byte _msgPin;
};

#endif
