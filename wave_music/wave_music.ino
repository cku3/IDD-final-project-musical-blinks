/*  Example using waveshaping to modify the spectrum of an audio signal
    using Mozzi sonification library.
  
    Demonstrates the use of WaveShaper(), EventDelay(), Smooth(),
    rand(), and fixed-point numbers.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.0/3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <EventDelay.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
#include <Smooth.h>
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <AutoMap.h> // maps unpredictable inputs to a range
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>


#include <tables/waveshape_chebyshev_3rd_256_int8.h>
#include <tables/waveshape_chebyshev_4th_256_int8.h>
#include <tables/waveshape_chebyshev_5th_256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_compress_1024_to_488_int16.h>
#include <tables/waveshape_compress_512_to_488_int16.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <Adafruit_LIS3DH.h>
//#include <Adafruit_Sensor.h>
//#define LIS3DH_CLK 13
//#define LIS3DH_MISO 12
//#define LIS3DH_MOSI 11
//// Used for hardware & software SPI
//#define LIS3DH_CS 10
//
//Adafruit_LIS3DH lis = Adafruit_LIS3DH();
//
//#define ENC_A 11 //these need to be digital input pins
//#define ENC_B 12

// ======================================== WaveShaper section =========================================== //
// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw(SAW2048_DATA); // sine wave sound source
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSawGain1(SAW2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSawGain2(SAW2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSawGain3(SAW2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSawGain4(SAW2048_DATA); // to fade sine wave in and out before waveshaping

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA); // sine wave sound source
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSinGain1(SIN2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSinGain2(SIN2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSinGain3(SIN2048_DATA); // to fade sine wave in and out before waveshaping
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSinGain4(SIN2048_DATA); // to fade sine wave in and out before waveshaping

// Chebyshev polynomial curves, The nth curve produces the n+2th harmonic component.
// total of 4 harmonics from 5-8
WaveShaper <char> aCheby3rd(CHEBYSHEV_3RD_256_DATA); // 5th harmonic
WaveShaper <char> aCheby4th(CHEBYSHEV_4TH_256_DATA); // 6th harmonic
WaveShaper <char> aCheby5th(CHEBYSHEV_5TH_256_DATA); // 7th harmonic
WaveShaper <char> aCheby6th(CHEBYSHEV_6TH_256_DATA); // 8th harmonic


// -244 to 243
WaveShaper <int> aCompress(WAVESHAPE_COMPRESS_1024_TO_488_DATA); // to compress instead of dividing by 2 after adding signals
WaveShaper <int> aCompressSaw(WAVESHAPE_COMPRESS_512_TO_488_DATA);

// for scheduling note changes
EventDelay kChangeNoteDelay;

// for random notes
Q8n0 octave_start_note = 42;
Q24n8 carrier_freq; // unsigned long with 24 integer bits and 8 fractional bits

// smooth transitions between notes
Smooth <unsigned int> kSmoothFreq(0.85f);
int target_freq, smoothed_freq;
//byte test_note = (byte)-2;
byte test_note = (byte)0;

// ================== The Following are for FM Modulation ================= //
// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);


Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);

int mod_ratio = 3; // harmonics
long fm_intensity; // carries control info from updateControl() to updateAudio()

// ======================SENSORS SECTION ============================//
// +++ ANALOG PINS +++ //
const int KNOB_PIN = 1; // set the input for the knob to analog pin 0
//const int LDR_PIN = 1; // set the input for the LDR to analog pin 1

// +++ DIGITAL PINS +++ //
const int buttonPinWAVE = 8;     // the number of the pushbutton pin
const int buttonPinFM = 10;     // the number of the pushbutton pin

const int LDR_PIN = 0;
//const int sclPin = A5;
//const int sdaPin = A4;
const int photoRes1 = 1;
const int photoRes2 = 2;
const int photoRes3 = 3;


void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  randSeed(); // reseed the random generator for different results each time the sketch runs
  Serial.begin(115200); // set up the Serial output so we can look at the piezo values // set up the Serial output for debugging

  // for Sawtooth osci
  aSaw.setFreq(110); // set the frequency
  aSawGain1.setFreq(2.f); // use a float for low frequencies, in setup it doesn't need to be fast
  aSawGain2.setFreq(.4f);
  aSawGain3.setFreq(.1f); // use a float for low frequencies, in setup it doesn't need to be fast
  aSawGain4.setFreq(.01f);

  // for Sine osci
  aSin.setFreq(110); // set the frequency
  aSinGain1.setFreq(2.f); // use a float for low frequencies, in setup it doesn't need to be fast
  aSinGain2.setFreq(.4f);
  aSinGain3.setFreq(.1f); // use a float for low frequencies, in setup it doesn't need to be fast
  aSinGain4.setFreq(.01f);

  kChangeNoteDelay.set(1000); // note duration ms, within resolution of CONTROL_RATE

//  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
//    Serial.println("Couldnt start");
//    while (1);
//  }
//  
//  lis.setRange(LIS3DH_RANGE_4_G);
//  delay(1000);
//  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
//  Serial.println("G");
//  /* Set up other input pins */
//  pinMode(ENC_A, INPUT_PULLUP);
//  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(buttonPinWAVE, INPUT);
  pinMode(buttonPinFM, INPUT);

  
  
}

//int read_encoder()
//{
//  static int enc_states[] = {
//    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
//  static byte ABab = 0;
//  ABab *= 4;                   //shift the old values over 2 bits
//  ABab = ABab%16;      //keeps only bits 0-3
//  ABab += 2*digitalRead(ENC_A)+digitalRead(ENC_B); //adds enc_a and enc_b values to bits 1 and 0
//  return ( enc_states[ABab]);
//}

byte rndPentatonic_test(byte note){
  switch(note){
  case 0:
    note = -2;
    break;
  case 1:
    note = 0;
    break;
  case 2:
    note = 2;
    break;
  case 3:
    note = 3;
    break;
  case 4:
    note = 5;
    break;
  case 5:
    note = 7;
    break;
  case 6:
    note = 9;
    break;
  case 7:
    note = 10;
  }
  return note;
}

void updateControl(){
  
  int light_level= mozziAnalogRead(KNOB_PIN); // value is 0-1023
  if (light_level>= 0 && light_level<128){
    test_note = 0;
  }
  else if (light_level >=128 && light_level<128*2){
    test_note = 1;
  }
  else if (light_level >=128*2 && light_level<128*3){
    test_note = 2;
  }
  else if (light_level >=128*3 && light_level<128*4){
    test_note = 3;
  }
  else if (light_level >=128*4 && light_level<128*5){
    test_note = 4;
  }
  else if (light_level >=128*5 && light_level<128*6){
    test_note = 5;
  }
  else if (light_level >=128*6 && light_level<128*7){
    test_note = 6;
  }
  else if (light_level >=128*7 && light_level<128*8){
    test_note = 7;
  }

  int oct_level= mozziAnalogRead(LDR_PIN); // value is 0-1023
  if (oct_level>= 0 && oct_level<10){
    octave_start_note = ((byte)1*12)+36;
  }
  else if (oct_level >=10 && oct_level<20){
    octave_start_note = ((byte)2*12)+36;
  }
  else if (oct_level >=20 && oct_level<100){
    octave_start_note = ((byte)3*12)+36;
  }
  else if (oct_level >=100){
    octave_start_note = ((byte)4*12)+36;
  }
  Serial.print("Sensor Data = "); 
  Serial.print(oct_level); 
  Serial.print("\t"); // prints a tab

  if(kChangeNoteDelay.ready()){ 
    Q16n16 midi_note = Q8n0_to_Q16n16(octave_start_note + rndPentatonic_test(test_note));
    target_freq = Q16n16_to_Q16n0(Q16n16_mtof(midi_note)); // has to be 16 bits for Smooth
    kChangeNoteDelay.start();
  }
  smoothed_freq = kSmoothFreq.next(target_freq*4); // temporarily scale up target_freq to get better int smoothing at low values
  aSaw.setFreq(smoothed_freq/4); // then scale it back down after it's smoothed
  aSin.setFreq(smoothed_freq/4); // then scale it back down after it's smoothed

  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(light_level);
  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio;
  // set the FM oscillator frequencies to the calculated values
  aCarrier.setFreq(carrier_freq); 
  aModulator.setFreq(mod_freq);
  fm_intensity = kMapIntensity(oct_level);
  

}


int updateAudio(){
  int buttonState_WAVE = digitalRead(buttonPinWAVE);
  int buttonState_FM = digitalRead(buttonPinFM);
  if (buttonState_FM == 0){
    if (buttonState_WAVE == 1){
        char asig0 = aSaw.next(); // saw wave source
        byte asig1 = (byte)128+((asig0*((byte)128+aSawGain1.next()))>>8);
        byte asig2 = (byte)128+((asig0*((byte)128+aSawGain2.next()))>>8);
  //      byte asig3 = (byte)128+((asig0*((byte)128+aSawGain3.next()))>>8);
  //      byte asig4 = (byte)128+((asig0*((byte)128+aSawGain4.next()))>>8);
        char awaveshaped1 = aCheby3rd.next(asig1);
        char awaveshaped2 = aCheby4th.next(asig2);
  //      char awaveshaped3 = aCheby5th.next(asig3);
  //      char awaveshaped4 = aCheby6th.next(asig4);
       
        
        // use a waveshaping table to squeeze 2 summed 8 bit signals into the range -244 to 243
        int awaveshaped3 = aCompressSaw.next(awaveshaped1+  awaveshaped2);
        //int awaveshaped5 = aCompress.next( awaveshaped1 + awaveshaped2);
        return awaveshaped3;
    }
    else {
        char asig3 = aSin.next(); // sine wave source
        byte asig4 = (byte)128+((asig3*((byte)128+aSinGain1.next()))>>8);
        byte asig5 = (byte)128+((asig3*((byte)128+aSinGain2.next()))>>8);
        byte asig6 = (byte)128+((asig3*((byte)128+aSinGain3.next()))>>8);
        byte asig7 = (byte)128+((asig3*((byte)128+aSinGain4.next()))>>8);
        char awaveshaped4 = aCheby3rd.next(asig4);
        char awaveshaped5 = aCheby4th.next(asig5);
        char awaveshaped6 = aCheby5th.next(asig6);
        char awaveshaped7 = aCheby6th.next(asig7);
       
        
        // use a waveshaping table to squeeze 2 summed 8 bit signals into the range -244 to 243
        int awaveshaped8 = aCompress.next( awaveshaped4 + awaveshaped5 + awaveshaped6 + awaveshaped7);
        //int awaveshaped5 = aCompress.next( awaveshaped1 + awaveshaped2);
        return awaveshaped8;
    }    
  }
  else{
    long modulation = fm_intensity * aModulator.next(); 
    return aCarrier.phMod(modulation); // phMod does the FM
  }
}


void loop(){
  audioHook(); // required here
}
