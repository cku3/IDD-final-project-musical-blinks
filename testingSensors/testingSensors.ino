/* 
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define ENC_A 11 //these need to be digital input pins
#define ENC_B 12

int button3pin = 9;
int button1pin = 10;

int softPotPin = A3;
int sclPin = A5;
int sdaPin = A4;
int photoRes1 = A0;
int photoRes2 = A1;
int photoRes3 = A2;
void setup()
{
  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  delay(1000);
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  
  lis.setRange(LIS3DH_RANGE_4_G);
  delay(1000);
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
  /* Set up other input pins */
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  
  pinMode(button3pin, INPUT);
  pinMode(button1pin, INPUT);
  pinMode(softPotPin, INPUT);
  pinMode(photoRes1, INPUT);
  pinMode(photoRes2, INPUT);
  pinMode(photoRes3, INPUT);
}

int read_encoder()
{
  static int enc_states[] = {
    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
  static byte ABab = 0;
  ABab *= 4;                   //shift the old values over 2 bits
  ABab = ABab%16;      //keeps only bits 0-3
  ABab += 2*digitalRead(ENC_A)+digitalRead(ENC_B); //adds enc_a and enc_b values to bits 1 and 0
  return ( enc_states[ABab]);
}

void loop()
{
  int photo1 = analogRead(photoRes1);
  int photo2 = analogRead(photoRes2);
  int photo3 = analogRead(photoRes3);
  int softPot = analogRead(softPotPin);
  int button1 = digitalRead(button1pin);
  int button3 = digitalRead(button3pin);
  Serial.println("photoresistor1: "); Serial.println(photo1); //550-825
  lis.read();
  //lis.x - x tilt
  //lis.y - y tilt
  //lis.z - z tilt
  //Serial.println("X:  "); Serial.println(lis.x); 
  //Serial.println("  \tY:  "); Serial.println(lis.y); 
  //Serial.println("  \tZ:  "); Serial.println(lis.z); 
  static unsigned int counter4x = 0;      //the SparkFun encoders jump by 4 states from detent to detent
  static unsigned int counter = 0;
  static unsigned int prevCounter = 0;
  int tmpdata;
  tmpdata = read_encoder();
  if( tmpdata) {
    counter4x += tmpdata;
    counter = counter4x/4;
    /*
    if (prevCounter != counter){
      Serial.print("encoder: ");
      Serial.println(counter);
    }
    */
    prevCounter = counter;
    
  
  //Serial.println("photoresistor2: "); Serial.println(photo2); 480-840
  //Serial.println("photoresistor3: "); Serial.println(photo3); //400-750 value range
  //Serial.println("soft pot: "); Serial.println(softPot); //0-1000
  //Serial.println("button1: "); Serial.println(button1); //0 or 1
  //Serial.println("button3: "); Serial.println(button3); //0 or 1
  delay(100);
  }
}
