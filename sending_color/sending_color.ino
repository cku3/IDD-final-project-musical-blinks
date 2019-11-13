void setup() {

  // No setup needed for this simple example!  

}

byte counter=0;

void loop() {

  if (buttonPressed()) {

    counter = counter + 1;

    if ( counter >= 3 ) {

      counter = 0; 
      
    }
  }

  if (counter==0) {
    setColor(RED);
    setValueSentOnAllFaces(10);
  } else if (counter==1) {
    setColor(GREEN); 
    setValueSentOnAllFaces(30);   
  } else { // if (counter==2)
    setColor(BLUE); 
    setValueSentOnAllFaces(60);
  }

}
