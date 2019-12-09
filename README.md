# IDD-final-project-Otakotone

## Project Idea: Otakotone
*Using Arduino to create a simple music synthesizer and editor*

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

## Interaction Plan:
Upon connection, the arduino will output sound signal. The user can interact with all the included sensors (detials see following) to modify and distort the sound and hear it from the speaker. The user can also view a music visualization from the browser that interacts with user input in real-time.

#### Functions of sensors:
