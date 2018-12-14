/*
With Thanks to code from Martin Koch
SIMPLE LANC REMOTE
Version 1.0
Sends LANC commands to the LANC port of a video camera.
Tested with a Canon XF300 camcorder
For the interface circuit interface see 
http://controlyourcamera.blogspot.com/2011/02/arduino-controlled-video-recording-over.html
Feel free to use this code in any way you want.
2011, Martin Koch

"LANC" is a registered trademark of SONY.
CANON calls their LANC compatible port "REMOTE".
*/


/*

 Copyright (c) 2013.  All rights reserved.
 An Open Source Arduino based LANC controller
 
  Program    : LANC
  Version    : v0.01, 28.3.2013
  Author     : Christof Schmid

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 http://www.gnu.org/licenses

/* ************************************************************ */
/* **************** MAIN PROGRAM - MODULES ******************** */
/* ************************************************************ */
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))

#undef PSTR
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];}))

//#define LANCDEBUG

/* **********************************************/
/* ***************** INCLUDES *******************/
#include <FastSerial.h>
#include <math.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>
#include "LANC.h"              /* Configurations */
#include <digitalWriteFast.h>  /* Direct portmanipulation library to replace digitalWrite. This is faster and smaller in code */
#include "Arduino.h"

/* *************************************************/
/* ***************** DEFINITIONS *******************/

int cmdRepeatCount;
int bitDuration = 104; //Duration of one LANC bit in microseconds.

FastSerialPort0(Serial);  /* Our Uart port*/

/* **********************************************/
/* ***************** SETUP() *******************/
void setup()
{
 
 Serial.begin( 115200 ); 

 pinModeFast(lancPin, INPUT); //listens to the LANC line
 pinModeFast(cmdPin, OUTPUT); //writes to the LANC line
 
 pinModeFast(PWMinRec, INPUT); // Set RC input pins as such
 pinModeFast(PWMinZoom, INPUT);

 digitalWrite(cmdPin, LOW); //set LANC line to +5V
 //delay(5000); //Wait for camera to power up completly
 bitDuration = bitDuration - 8; //Writing to the digital port takes about 8 microseconds so only 96 microseconds are left for each bit


    /* Initialize LED output pins */

    pinModeFast(LedPin ,OUTPUT);
    startLED();

} /* end */

/* * * * * * * * *  MAIN LOOP * * * * * * * * * * */

void loop()
{
  ReadRC();
  SetCam();
  TimerEvent();
   
#ifdef LANCDEBUG  
//  Serial.flush();
  Serial.print( "Camera Kanal: " );
  Serial.print( RCOnOff ) ;
 
  Serial.print( "  Zoom Kanal: " );
  Serial.print( RCZoom ) ;
  
  Serial.print( "  ZoomSpeed: " );
  Serial.print( ZoomSpeed ) ;
  
  Serial.print( "  Cam Switch: " );
  Serial.print( SwitchPos ) ;  

  Serial.print( "  Cam Status: " );
  Serial.print( CamStat ) ;
 
  Serial.print( "  LED Status: " );
  Serial.println( LedStatus ) ;
  
  Serial.print( "  LANC Ready: " );
  Serial.print( LANCReady ) ;
 #endif 
  
  
  }






