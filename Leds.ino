/* LED patterns are hard coded to the flightstatus number but use your imagination to write your own patterns */

void startLED(){
    digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter ==1) ? HIGH : LOW);    /* high on ioCounter 0 or 5 pass, low on all other */
  }

void LEDControl() {

  switch (LedStatus) {
    
      if (LANCReady == 0) {
         digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter == 3 || ioCounter == 6 || ioCounter == 9  ) ? HIGH : LOW);
         break;
      }   
    
   case 0: //No RC
      digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter == 2 || ioCounter == 4 || ioCounter == 6 || ioCounter ==8 ) ? HIGH : LOW);
      //digitalWriteFast(LedPin, (ioCounter == 3 || ioCounter ==8) ? HIGH : LOW);
      break;
   case 1: //Cam Off
      digitalWriteFast(LedPin, LOW);
      break;
   case 2: //Cam On 
      digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter ==1) ? HIGH : LOW);
      break;
   case 3: // Recording
       //digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter ==5) ? HIGH : LOW);
      digitalWriteFast(LedPin, HIGH);
      break;   
      
      if (LANCReady == 0) {
         digitalWriteFast(LedPin, (ioCounter == 0 || ioCounter == 1 || ioCounter == 2 || ioCounter == 3  ) ? HIGH : LOW);
      }
      
   }
}


// Switch all outputs ON
void AllOn() {
  digitalWriteFast(LedPin, HIGH);

}

// Switch all outputs OFF
void AllOff() {
  digitalWriteFast(LedPin, LOW);

}




