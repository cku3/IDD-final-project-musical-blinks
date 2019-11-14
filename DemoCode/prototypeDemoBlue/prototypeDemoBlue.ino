void setup() {

  // No setup needed for this simple example!  

}

//byte counter=0;
byte blue = byte(0b00000100);
byte cyan = byte(0b00000011);
byte magenta = byte(0b00000101);
byte red = byte(0b00000110);

byte getHue(byte data) {
  return (data & 7);                //the value in the fourth, fifth, and sixth bits
}

bool onFlag = false;
Timer nextChange;
int onOff(int face){
  for(int t = 0; t<5; t++){
    if (nextChange.isExpired()) {
  
        setColorOnFace(BLUE, face);
        onFlag = true; 
  
      nextChange.set(500);   // Toggle every 500 milliseconds 
    }
  }
}

void loop() {
    //BLUE blink
    setColorOnFace(RED, 0); 
    setColorOnFace(BLUE, 1); 
    setColorOnFace(RED, 2); 
    setColorOnFace(BLUE, 3); 
    setColorOnFace(BLUE, 4); 
    setColorOnFace(BLUE, 5);
    
    //if a BLUE face is touching cyan, show white path
    //if it's touching green, flash those led faces
    int notBarriers[] = {1,3,4,5};
    int barriers[] = {0,2};
    for(int b = 0; b < 2; b++){
      setValueSentOnFace(red, barriers[b]);
    }
    for(int i=0;i<4;i++){
      setValueSentOnFace(blue, notBarriers[i]);
      if (!isValueReceivedOnFaceExpired(notBarriers[i])){
        byte neighborData = getLastValueReceivedOnFace(notBarriers[i]);
          if (getHue(neighborData) == cyan) {
            setColorOnFace(WHITE,notBarriers[i]);
            }
          else if (getHue(neighborData) == magenta){
            setColorOnFace(WHITE,notBarriers[i]);
          }
          else {
            if (getHue(neighborData) != red){
              setColorOnFace(OFF,notBarriers[i]);
              onOff(notBarriers[i]);
            }
          }          
       } 
    }
}
