void ReadRC(){
  RCOnOff = pulseIn(PWMinRec, HIGH, 25000); // Read the pulse width of 
  RCZoom  = pulseIn(PWMinZoom, HIGH, 25000); // each channel 
  RCOnOff = constrain( RCOnOff, MinPWM, MaxPWM);
  RCZoom  = constrain( RCZoom, MinPWM, MaxPWM);
  }
  
  
void SetCam(){
  ZoomPW   = RCZoom - 1500; // -500 bis +500
  int ZoomFact = 58;
  int OldSwitchPos = SwitchPos;
  ZoomSpeed    = ZoomPW / ZoomFact;
  ZoomSpeed    = abs(ZoomSpeed);
  ZoomSpeed    = constrain(ZoomSpeed,0,7);
  
  if ( (RCZoom == MaxPWM) || (RCOnOff == MaxPWM) || (RCZoom == MinPWM) || (RCOnOff == MinPWM) ) { // No Signal (min), disconnected (max) 
   LedStatus = 0;  
   ZoomSpeed = 0;
   return;
  }

//Zooming
 while (ZoomPW <= -50) {
   lancCommand(ZOOM_OUT[ZoomSpeed]);
   ReadZoom();  //for speedup, zoom hickups
 }

 while (ZoomPW >= 50) {
   lancCommand(ZOOM_IN[ZoomSpeed]);  
   ReadZoom();  //for speedup, zoom hickups
 }  
 

//SwitchPos 0,1,2
if ( (RCOnOff >= 900) && (RCOnOff <=1300) ) {
 SwitchPos = 0; 
}

if ( (RCOnOff > 1300) && (RCOnOff <=1700) ) {
 SwitchPos = 1;
}

if (RCOnOff > 1700) {
 SwitchPos = 2; 
}  



if ( SwitchPos == OldSwitchPos ) {
 return;
} 





//CamStat 0 = Off, 1 = on NoRec, 2 = on REC

  if ( SwitchPos == 0 ) {
    if ( CamStat > 0 ) {
       #ifdef LANCDEBUG
       Serial.print( "Power cam off " );
       Serial.println( "" ) ;
       #endif  
      PowerOff();
      CamStat = 0;
      //lancCommand(POWER_OFF); 
    }
  }
  
  if ( SwitchPos == 1 ) {

    if (CamStat == 0) {
       #ifdef LANCDEBUG
       Serial.print( "Power cam on " );
       Serial.println( "" ) ;
       #endif  
       PowerOn();
        //lancCommand(POWER_ON);
      }

      if (CamStat == 2) {
       #ifdef LANCDEBUG
       Serial.print( "Stop recording " );
       Serial.println( "" ) ;
       
       #endif          
       lancCommand(REC);  //Rec Aus
       delay(500);
      }
    
    CamStat = 1 ;
         
  }
 
 if ( SwitchPos == 2 ) {
       #ifdef LANCDEBUG
       Serial.print( "Start recording " );
       Serial.println( "" ) ;
       #endif  
      lancCommand(REC); //Rec Ein 
      delay(500);
      CamStat = 2 ;
  }
 
LedStatus = CamStat + 1;  
 
}  


void ReadZoom(){
  RCZoom  = pulseIn(PWMinZoom, HIGH, 25000); // each channel 
  RCZoom  = constrain( RCZoom, MinPWM, MaxPWM);
  ZoomPW   = RCZoom - 1500; // -500 bis +500
  int ZoomFact = 58;
  ZoomSpeed    = ZoomPW / ZoomFact;
  ZoomSpeed    = abs(ZoomSpeed);
  ZoomSpeed    = constrain(ZoomSpeed,0,7);  
}

void PowerOn() {
 pinModeFast(lancPin, OUTPUT); // LANC line, is LOW
 delay(150);
 pinModeFast(lancPin, INPUT); // LANC line, still LOW  
 delay(4000);

}

void PowerOff() {
lancCommand(POWER_OFF); 
LANCReady = 0;
delay(6000);
  
}

void TimerEvent() {  /* this event is called @ 10Hz */

TimerEnd += (millis() - TimerEnd) ;
  if (TimerEnd > (TimerStart + 100)) {
     ioCounter ++ ;     /* update counter to use in all blink loops */
     if (ioCounter == 10) ioCounter =0;
     
     
     LEDControl();
     
     
     //Serial.println( "Timer Event " );
  TimerStart = millis() ;
  TimerEnd = millis() ;
  }
}



void lancCommand(boolean lancBit[]) {
       
   cmdRepeatCount = 0;
   
   #ifdef LANCDEBUG
     return;
   #endif
   
if ( CamStat == 0 ){
  return;
}
   

  while (cmdRepeatCount < 5) {  //repeat 5 times to make sure the camera accepts the command

   while (pulseIn(lancPin, HIGH) < 5000) {
   //"pulseIn, HIGH" catches any 0V TO +5V TRANSITION and waits until the LANC line goes back to 0V
   //"pulseIn" also returns the pulse duration so we can check if the previous +5V duration was long enough (>5ms) to be the pause before a new 8 byte data packet
   //Loop till pulse duration is >5ms
    if (LANCReady == 0) {
     #ifdef LANCDEBUG
       Serial.print( "Waiting for LANC Pin to go high : " );
       Serial.println( LANCReady ) ;
     #endif  
     TimerEvent();         
    }
   }
   
   LANCReady = 1; 
    
   //LOW after long pause means the START bit of Byte 0 is here
   delayMicroseconds(bitDuration);  //wait START bit duration

   //Write the 8 bits of byte 0
   //Note that the command bits have to be put out in reverse order with the least significant, right-most bit (bit 0) first
   for (int i=7; i>-1; i--) {
     digitalWrite(cmdPin, lancBit[i]);  //Write bits.
     delayMicroseconds(bitDuration);
   }

   //Byte 0 is written now put LANC line back to +5V
   digitalWrite(cmdPin, LOW);
   delayMicroseconds(10); //make sure to be in the stop bit before byte 1
   while (digitalRead(lancPin)) {
     //Loop as long as the LANC line is +5V during the stop bit
   }

   //0V after the previous stop bit means the START bit of Byte 1 is here
   delayMicroseconds(bitDuration);  //wait START bit duration
   //Write the 8 bits of Byte 1
   //Note that the command bits have to be put out in reverse order with the least significant, right-most bit (bit 0) first
   for (int i=15; i>7; i--) {
     digitalWrite(cmdPin,lancBit[i]);  //Write bits
     delayMicroseconds(bitDuration);
   }
   //Byte 1 is written now put LANC line back to +5V
   digitalWrite(cmdPin, LOW); 
   cmdRepeatCount++;  //increase repeat count by 1
   
   /*Control bytes 0 and 1 are written, now dont care what happens in Bytes 2 to 7
   and just wait for the next start bit after a long pause to send the first two command bytes again.*/

 } //While cmdRepeatCount < 5
}




