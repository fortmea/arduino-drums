# ArduBeats
This is a symple arduino project using a 16x2 LCD and 4 buttons. 
The goal here is to develop a simple, easy to make and use lo-fi drum machine.

## Characteristics
- Menu-based GUI;
- 4 buttons - Cycle menus, positive action, negative action, play-pause / skip beat slot / cycle save-load slot.
- Save/Load 4 beats to the built-in EEPROM on the arduino.
- Pitch control.
- BPM control.

## Showcase
Coming soon.

## Pictures

<div style="display: flex; flex-wrap: wrap; gap: 10px;">
    <a href="https://github.com/fortmea/arduino-drums/blob/main/project%20images/bpmcontrol.JPEG?raw=true">
        <img src="https://github.com/fortmea/arduino-drums/blob/main/project%20images/bpmcontrol.JPEG?raw=true" width="250" style="max-height: 200px;" alt="BPM CONTROL"/>
    </a>
    <a href="https://github.com/fortmea/arduino-drums/blob/main/project%20images/kickcontrol.JPEG?raw=true">
        <img src="https://github.com/fortmea/arduino-drums/blob/main/project%20images/kickcontrol.JPEG?raw=true" width="250" style="max-height: 200px;" alt="KICK DRUM CONTROL"/>
    </a>
    <a href="https://github.com/fortmea/arduino-drums/blob/main/project%20images/pitchcontrol.JPEG?raw=true">
        <img src="https://github.com/fortmea/arduino-drums/blob/main/project%20images/pitchcontrol.JPEG?raw=true" width="250" style="max-height: 200px;" alt="PITCH CONTROL"/>
    </a>
    <a href="https://github.com/fortmea/arduino-drums/blob/main/project%20images/saveloadcontrol.JPEG?raw=true">
        <img src="https://github.com/fortmea/arduino-drums/blob/main/project%20images/saveloadcontrol.JPEG?raw=true" width="250" style="max-height: 200px;" alt="SAVE AND LOAD MENU"/>
    </a>
    <a href="https://github.com/fortmea/arduino-drums/blob/main/project%20images/welcome.JPEG?raw=true">
        <img src="https://github.com/fortmea/arduino-drums/blob/main/project%20images/welcome.JPEG?raw=true" width="250" style="max-height: 200px;" alt="WELCOME MENU"/>
    </a>
</div>

## Hardware
- Arduino Uno (R3 in this case), any compatible board should work - Used a FUNDUINO.
- 16x2 LCD (I used a 1602A) - I'm using the serial 4 bit mode.
- 4 * push buttons.
- A single 10k or 5k potentiometer (contrast control).
- Jumper wires.

## Schematics
![alt text](https://github.com/fortmea/arduino-drums/blob/main/ArduBeats_bb.jpg?raw=true)

## License
Copyright 2024 fortmea.tech

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the “Software”), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.