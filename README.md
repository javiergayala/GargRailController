# GargRail Controller #

## Introduction ##

This is one-half of my attempt at creating a more robust Disney Monorail toy with the use of Arduino Microcontrollers.  This code will be used to provide the functionality for the wireless controller portion of the project.

## Status ##
Still under development.  Working on creating a functional prototype.  At this time, the GargRailController.ino file contains the functionality that allows for reading of the speed from the linear pot, displaying that speed on the LCD, and taking input from the "Emergency Stop" and "Brake Release" buttons and taking the appropriate actions of sending a "stop" signal or allowing the monorail to start up again.

## Parts Required ##

* Arduino Microcontroller (I'm using the Arduino Uno R3)
* XBee Wireless Radio (I'm using the Series 1)
* 16x2 LCD Display (and necessary hardware)
* Linear Potentiometer (Provides input for speed control)
* 2 Momentary Switch Buttons (Emergency Stop and Brake Release)
* Red LED
* Green LED
* Capacitors and Resistors as needed (still working out the specs)