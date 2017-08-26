# LidarCruise

Lidar Cruise is a level 1 autonomous active cruise control system for my 2005 Mazda6. By interfacing with my steering wheel cruise control buttons and a Lidar unit, the distance to the object ahead can be varied.

Included in this Repo:

## 2 Matlab files: 
These were used during conceptualization of the project. Although not used on the final hardware for various reasons, they provided valuable knowledge at the start. Scenecreation.m builds one of three environments based off my hand drawings of satellite images. Basically creates a 2d model for the next file to interact with. ACC_code_tests.m identifies objects based on size, angle, and position. Very matrix heavy, probably too much to run on an Arduino. Hence, simplified code for final arduino hardware.

## 4 Arduino Ino files:
These run on the 4 arduinos that make up Lidar Cruise

LidarACC:

Central brain, also called Arduino 3

VSS_ACC: 

Reads VSS wire, passes info to central brain upon request, also called Arduino 2

MirrorACC:

Controls motor that Lidar is attached to. Sends trigger to central brain when photogate is hit, starting main code. Essentially a clock for the entire system. Also called Arduino 1

ControlACC:

Controls relays attached to resistors. Reads from steering wheel and central brain, decides what command to send to car cruise control module. Based on SteeringWheelControl code. Also called Arduino 4.

## 1 Fritzing file: 
This file allowed planning of wiring for Lidar Cruise

## 1 Blender file: 
This object was 3D printed, allowing waterproof connection of the Lidar tube and flat window. Needed since the Lidar was not looking through a curved window directly attached to the tube. 

##

Please note: This code is provided for informational use only. Don't do anything stupid with it that may harm anyone or anything. Remember, a level 1 autonomous system requires driver interaction at all times, who is ultimately responsible for the vehicle.
