#include "Arduino.h"
void setup() {

  // No setup needed for this simple example!  
}
byte green = byte(0b00000010);
byte blue = byte(0b00000100);
byte cyan = byte(0b00000011);
byte red = byte(0b00000110);

byte getHue(byte data) {
  return (data & 7);                //the value in the fourth, fifth, and sixth bits
}
//bool wrong = false;
//bool onFlag = false;
//Timer nextChange;
//void onOff(){
//  while(wrong){
//    if (nextChange.isExpired()) {
//
//    if (onFlag) {
//
//      setColor( OFF );      
//      onFlag = false;
//
//    } else {
//
//      setColor(BLUE);
//      onFlag = true; 
//      
//    }
//
//    nextChange.set(500);   // Toggle every 500 milliseconds 
//  }
//}
//}
void loop() {
    //CYAN blink 
    setColorOnFace(CYAN, 0); 
    setColorOnFace(RED, 2); 
    setColorOnFace(CYAN, 1); 
    setColorOnFace(CYAN, 3); 
    setColorOnFace(CYAN, 4); 
    setColorOnFace(RED, 5); 
    
    int barriers[] = {2,5};
    for(int b = 0; b < 2; b++){
      setValueSentOnFace(red, barriers[b]);
    }
    
    int notBarriers[] = {0,1,3,4};    
    //if a cyan face (0,1,3,4) is touching green or blue, display white path 
    for (int i = 0; i< 4; i++) {
      //wrong = false;
      //sending out values for other blinks to receive
      setValueSentOnFace(cyan, notBarriers[i]);
      if (!isValueReceivedOnFaceExpired(notBarriers[i])){
        byte neighborData = getLastValueReceivedOnFace(notBarriers[i]);
          if (getHue(neighborData) == green) {
            setColorOnFace(WHITE,notBarriers[i]);
            }
          else if (getHue(neighborData) == blue){
            setColorOnFace(WHITE,notBarriers[i]);
          }
          else {
            //setColorOnFace(YELLOW,notBarriers[i]);
          }          
       } 
    }
  
}
