# IDD-final-project-musical-blinks

## Project Idea: MusicalBlinks
*Using Move38's Blinks to create a simple music synthesizer and editer*

We pivoted from this idea because there were too few ports to access for connecting the speaker.
Plus, the tone/Mozzi libraries need access to an internal timer, but all three were occupied by other processes.

## Team:
* Christine Ku (ECE)
* Yuxin Zhang (ECE)

## Paper Prototype:
![paperproto](https://i.imgur.com/M9wTFU8.jpg)

Some other research into Arduino's melody library: 

https://www.arduino.cc/en/Tutorial/PlayMelody

create some melodies to select from

can change tempo and duration of notes

## Expected Parts:
* 6 Blinks boards (custom Arduinos with built in buttons and LEDs)
* Lithium ion coin batteries
* Speaker
* Wires
* Arduino

## Interaction Plan:
To initialize this game/synthesizer, you have to connect all the hexagons together and hold down the center button of one to allow comunication between all of them. Once that is set, they can be broken apart and different settings can be selected (Each hexagon will initialize as a different color to indicate the differences between the different settings to select):

* Select song on one hexagon: colors will flash and the user will click the center button to select a tune
* Select a tempo: colors will flash and the user will click the center button to select speed of tune
* Select a reverb level: colors will flash and the user will click the center button to select how much reverb they want
* Select a distortion level: colors will flash and the user will click the center button to select how much distortion they want
* Click center button of another hexagon to turn on/off the sound
* Click center button of another hexagon to loop tune or not

Once the user connects certain hexagons together and turns on the sounding hexagon, the modified tune will play.
