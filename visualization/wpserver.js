/*
server.js

Authors:David Goedicke (da.goedicke@gmail.com) & Nikolas Martelaro (nmartelaro@gmail.com)

This code is heavily based on Nikolas Martelaroes interaction-engine code (hence his authorship).
The  original purpose was:
This is the server that runs the web application and the serial
communication with the micro controller. Messaging to the micro controller is done
using serial. Messaging to the webapp is done using WebSocket.

//-- Additions:
This was extended by adding webcam functionality that takes images remotely.

Usage: node server.js SERIAL_PORT (Ex: node server.js /dev/ttyUSB0)

Notes: You will need to specify what port you would like the webapp to be
served from. You will also need to include the serial port address as a command
line input.
*/

var express = require('express'); // web server application
var app = express(); // webapp
var http = require('http').Server(app); // connects http library to server
var io = require('socket.io')(http); // connect websocket library to server
var serverPort = 5000;
var SerialPort = require('serialport'); // serial library
var Readline = SerialPort.parsers.Readline; // read serial data as lines

//---------------------- WEBAPP SERVER SETUP ---------------------------------//
// use express to create the simple webapp;
app.use(express.static('app'));

// check to make sure that the user provides the serial port for the Arduino
// when running the server
if (!process.argv[2]) {
  console.error('Usage: node ' + process.argv[1] + ' SERIAL_PORT');
  process.exit(1);
}

// start the server and say what port it is on
http.listen(serverPort, function() {
  console.log('listening on *:%s', serverPort);
});
//----------------------------------------------------------------------------//


//---------------------- SERIAL COMMUNICATION (Arduino) ----------------------//
// start the serial port connection and read on newlines
const serial = new SerialPort(process.argv[2], {});
const parser = new Readline({
  delimiter: '\r\n'
});

// Read data that is available on the serial port and send it to the websocket
serial.pipe(parser);
parser.on('data', function(data) {
  console.log('Update:', data);
  io.emit('update', data);
});
//----------------------------------------------------------------------------//


const readline = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
  })
  
  

//---------------------- WEBSOCKET COMMUNICATION (web browser)----------------//
// this is the websocket event handler and say if someone connects
// as long as someone is connected, listen for messages
io.on('connect', function(socket) {
  console.log('a user connected');

  process.stdin.on('readable', () => {
    let chunk;
    // Use a loop to make sure we read all available data.
    while ((chunk = process.stdin.read()) !== null) {
      process.stdout.write(`data: ${chunk}`);
      io.emit('update', `${chunk}` + ';15;20;0;4.5');
    }
  });

  // if you get the 'disconnect' message, say the user disconnected
  socket.on('disconnect', function() {
    console.log('user disconnected');
  });
});
//----------------------------------------------------------------------------//
