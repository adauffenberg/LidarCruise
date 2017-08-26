# LidarCruise

Lidar Cruise is a level 1 autonomous active cruise control system for my 2005 Mazda6. By interfacing with my steering wheel cruise control buttons and a Lidar unit, the distance to the object ahead can be varied.

Included in this Repo:

## 2 Matlab files: 
These were used during conceptualization of the project. Although not used on the final hardware for various reasons, they provided valuable knowledge at the start. Scenecreation.m builds one of three environments based off my hand drawings of satellite images. Basically creates a 2d model for the next file to interact with. ACC_code_tests.m identifies objects based on size, angle, and position. Very matrix heavy, probably too much to run on an Arduino. Hence, simplified code for final arduino hardware.

## 4 Arduino Ino files:
These run on the 4 arduinos that make up Lidar Cruise

### Ino to Website guide

LidarACC is Arduino 3

VSS_ACC is Arduino 2

MirrorACC is Arduino 1

ControlACC is Arduino 4

## 1 Fritzing file: 
This file allowed planning of wiring for Lidar Cruise

## 1 Blender file: 
This object was 3D printed, allowing waterproof connection of the Lidar tube and window.
