Arduino serial and raspberry pi serial reader should communicate according to the following string format:

`value1;value2;value3;value4;value5`

and the values must follow the ordering:

*FM/WaveShaper;Octave;Volume;Sine/SawTooth;Pitch*

e.g. `1;15;20;0;4.5`