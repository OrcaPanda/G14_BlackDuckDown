# G14_BlackDuckDown
ECE532 Capstone Design Project

## Project Description
The improvement of audio processing methods in recent years has given rise to a variety of applications, most notably in the software space. Some of these applications fall into the ‘voice-training’ category, where the user produces attempts to produce sounds that fulfill some criteria, and the application provides feedback to the user on how well they were able to fulfill that criteria.

The goal of this project was to implement a similar application on an FPGA, and to wrap that functionality into an engaging game to prolong the player’s interest and present more of a challenge. In this project, we have taken inspiration from the gameplay dynamics of both ‘Duck Hunt’ and ‘Guitar Hero’. 

Our project, called ‘Black Duck Down’, is a real-time voice-training game where the player must sing specific notes in order to score points. This is game-ified by representing the notes as ducks and representing wins as killing the corresponding ducks. 

Similar to existing applications, this game can be used to train the player’s pitch recognition and voice stability. During gameplay, ducks appear at random on the screen, each associated with a unique note in the range C2-C5. Players who are able to find the correct pitches faster, score higher in the game.

## How to use
1. Git clone this repository
2. Repackage frequency_calculation_core and quacker to use absolute paths on your computer
3. Synthesize with the game.v and game_wrapper.v Verilog files  
  \- OR \-
3. Create a new project and include the IP blocks repositories that you need

## Repository structure
|- README.md  
|- docs/  
|........|- Presentation Slides.pdf  
|........|- Final Report  
|  
|- src/  
|........|- PERIPHERALS/  
|........|........|- display_3.0/  
|........|........|- frequency_calculation_core/  
|........|........|- hexprint/  
|........|........|- pushbutton/  
|........|........|- quacker/  
|........|........|- constraints.xdc  
|........|........|- FHT_LUT_VALUES.txt  
|........|........|- FHT_TEST_VALUES.txt  
|........|........|- ZERO_VALUES.txt  
|........|  
|........|- SOFTWARE/  
|........|........|- main.c  
|........|........|- duck_list.h  
|........|........|- definitions.h  
|........|  
|........|- game.v  
|........|- game_wrapper.v  
|  

## Authors
Jianwei Sun  
Wenyi Yin  
Wen Bo Li  

## Acknowledgements
Black Duck Down could not have been created without the generous support and guidance of Professor Paul Chow and all of the teaching assistants in ECE532 2017 Spring. Our team would like to thank our TA, Jin Hee, for her continued support throughout this project.
