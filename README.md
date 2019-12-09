# IDD-final-project-Otakotone

## Project Idea: Otakotone
*After a couple iterations, we decided to create a simple music synthesizer and editor using Arduino with a visualization implemented through the Raspberry Pi*

Project name is inpired by the fun electronic music synthesizer [Otamatone](https://en.wikipedia.org/wiki/Otamatone) and Yuxin's avid love for tako (octopus).

## Team:
* Christine Ku (ECE)
* Yuxin Zhang (ECE)
* Fei Gao (CS)

## Paper Prototype:
![paperproto](./otakotone.jpg)

#### Library we are using: 
[Mozzi](https://sensorium.github.io/Mozzi/) to generate algorithmic music on an arduino board

[Three.js](https://threejs.org/) for 3D object visualization

[Websocket.io](http://websocket.io/) for sending music data from arduino to raspberry pi

[Node.js](https://nodejs.org/en/) for backend webserver

## Expected Parts:
* Speaker
* Buttons
* Wires
* Arduino
* Raspberry Pi
* Potentiometer
* Accelerometer
* Flex Sensor
* Pressure Sensor
* Cardboard
* Breadboard

## Interaction Plan:
Upon connection, the Arduino outputs some sound signal. The user can interact with all the included sensors (details below) to modify and distort the sound emitted from the speaker. The user can also view a music visualization from the browser that interacts with user input in real-time.

#### Functions of sensors:
* SoftPot: By pressing down on different parts of the strip, the user can change the **pitch** of the sound output.
* Encoder: The user can increase or lower the **octave** of the output by pressing down on the encoder and twisting.
* Yellow Button: This button, when pressed, changed the output to implement **FM Modulation**.
* Red Button: When this button is pressed, the output is changed to a **sawtooth wave** instead of a sine, which almost sounds like there is a beat!
* Photoresistor: This sensor was incompatible with the rest of our system but in the prototype phase, it was used to change **volume**. However, it is not implemented in our housed product.

## Functional Prototype:

We first created our synthesizer using a breadboard in order to make sure the functionality was there. The images below show the breadboard circuitry for testing the sensors and the Mozzi library separately.

#### Testing Sensors:
![Testing Sensors](https://i.imgur.com/lS1iuzc.jpg?1)

#### Testing Mozzi Library:
![Testing Mozzi Library](https://i.imgur.com/Hg0AcFP.jpg?1)

We ran into a couple issues with some of the combinations of sensors with the Mozzi library. For example, the photoresistors created a sort of hang in the serial reading, so we decided to do without it. Furthermore, for the accelerometer although it worked when testing all the sensors at once, the Mozzi library used the same internal timer, so the LIS3DH would not be able to be accessed without using a separate Arduino board. Below, we have a video of our functional synth below:

[![Mozzi Synth Video](https://i9.ytimg.com/vi/MYVobK3GI_c/mq2.jpg?sqp=CPubuu8F&rs=AOn4CLCWQiglEU78GqJM8DSfsaF6UQWfvw)](https://youtu.be/MYVobK3GI_c)

## Housing:

In order to make our product look more like a tako, we decided to add 4 "legs" and a head in which the sensors are housed. First, we transferred our circuit onto a protoboard and made sure all the connections were correct. The MetroMini is removable for reused and easy debugging from breadboard to protoboard.

![Transferring to Protoboards](https://i.imgur.com/qV4iF5n.jpg)

Our box is a little unconventional compared to the normal rectangular box so we laser cut three different designs using [this link](https://boxdesigner.connectionlab.org/), with the same notch size so they could fit together seamlessly into our desired shape. Then we started assembly as shown below:

![Laser Cut Box Assembly](https://i.imgur.com/XAflrkJ.jpg)

The .svg files are included [here]().

## Final Product:

[![Visualization]()]()
[![Housed Synth]()]()
