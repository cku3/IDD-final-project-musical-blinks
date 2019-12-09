void setup() {

  // No setup needed for this simple example!  

}

//byte counter=0;
byte red = byte(0b00000110);
byte cyan = byte(0b00000011);
byte yellow = byte(0b00000001);
byte green = byte(0b00000010);

byte getHue(byte data) {
  return (data & 7);                //the value in the fourth, fifth, and sixth bits
}

//toggle mismatched face
bool onFlag = false;
Timer nextChange;
int onOff(int face){
  for(int t = 0; t<5; t++){
    if (nextChange.isExpired()) {
        setColorOnFace(GREEN, face);
        onFlag = true;   
      nextChange.set(500);   // Toggle every 500 milliseconds 
    }
  }
}

void loop() {
    //GREEN blink
    setColorOnFace(GREEN, 0); 
    setColorOnFace(GREEN, 1); 
    setColorOnFace(RED, 2); 
    setColorOnFace(RED, 3); 
    setColorOnFace(GREEN, 4); 
    setColorOnFace(GREEN, 5);
    //if a green face (0,1,4,5) is touching a cyan face, display white path
    //if it's touching a blue face, flash that point to indicate that's illegal
    int barriers[] = {2,3};
    for(int b = 0; b < 2; b++){
      setValueSentOnFace(red, barriers[b]);
    }
    int notBarriers[] = {0,1,4,5};
    for(int i=0;i<4;i++){
      setValueSentOnFace(green, notBarriers[i]);
      if (!isValueReceivedOnFaceExpired(notBarriers[i])){
        byte neighborData = getLastValueReceivedOnFace(notBarriers[i]);
          if (getHue(neighborData) == cyan) {
            setColorOnFace(WHITE,notBarriers[i]);
            }
          else if (getHue(neighborData) == yellow){
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
