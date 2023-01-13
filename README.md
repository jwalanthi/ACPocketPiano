# ACPocketPiano
Creative code for an Animal Crossing-Themed embedded system video game based on Piano Tiles

This game was designed to be played on a TM4C microcontroller interfaced with 6 switches, an ADC-enabled potentiometer (inputs),
an ST7735 LED screen, and a DAC-enabled speaker (outputs).

This repository does not have all the code necessary to run such code, as some portions, such as the drivers and the timer modules, contain proprietary information.
Instead, this repository contains the creative code developed by myself and my lovely lab parter, Phebe Tan. Below is a brief description of each file:

## PocketPiano.c: 
this is the "main" file. It initializes all the hardware components, displays the intro screen,then goes through each level of the game. The FallingKeys function is 
the ISR for Timer1 and is responsible for the movement of the rectangle sprites. The other functions have pretty intuitive names.

## Sound.h: 
contains #define's for timer interrupts for given sine-wave pitches, declarations for note and song structs, and function declarations for Sound.c

## Sound.c: 
Defines song structs for each gameplay level, DAC outputs for a sine wave, and function definitions having to do with sound output to DAC speaker

## Buttons.h: 
declares button functions

## Buttons.c: 
Defines ISR for edge-triggered interrupts from the 6 switches to measure user input for main to score

a video of the gameplay can be found on youtube: https://youtu.be/8fhWFKvyDPg
