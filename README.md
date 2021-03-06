# LidarCruise

Lidar Cruise is a level 1 autonomous active cruise control system for my 2005 Mazda6. By interfacing with my steering wheel cruise control buttons and a Lidar unit, the distance to the object ahead can be varied.

Pleae read this projects website first for a overview of the system. This will aid in understanding of the files here.

Included in this Repo:

## 2 Matlab files and jpg image: 
These were used during conceptualization of the project. Although not used on the final hardware for various reasons, they provided valuable knowledge at the start. Scene_creation.m builds a environment based off my manual graphing of green lines on satellite image. Basically creates a 2d model for the next matlab file to interact with. ACC_code_tests.m runs a "virtual lidar" by firing blue beams, collisions are red points. Objects from scene_creation.m are identified based on size, angle, and position of the red points and are marked in white. This code is very matrix heavy, probably too much to run on an Arduino. Hence, a much simplified code is run on final arduino hardware.

## 4 Arduino Ino files:
These run on the 4 arduinos that make up Lidar Cruise

LidarACC:

Central brain. Gathers Lidar data and data from other arduinos. Also called Arduino 3

VSS_ACC: 

Reads VSS wire, passes vehicle speed to central brain upon request, also called Arduino 2

MirrorACC:

Controls motor that Lidar is mounted on, as well as reading photogates. Sends trigger to central brain when photogate is triggered from motor rotation, starting main loop in central brain. Essentially a clock for the entire system. Also called Arduino 1.

ControlACC:

Reads from steering wheel buttons and central brain, decides what relay/resistor to switch. This controls the car cruise control module set speed. Based on my SteeringWheelControl repo code. Also called Arduino 4.

## 1 Fritzing file: 
This file allowed planning of wiring for Lidar Cruise

## 1 Blender file: 
This object was 3D printed, allowing waterproof connection of the Lidar tube and flat window. Needed since the Lidar was not looking through a curved window directly attached to the tube. 

##

Please note: This code is provided for informational use only. Don't do anything stupid with it that may harm anyone or anything. Remember, a level 1 autonomous system requires driver interaction at all times, who is ultimately responsible for the vehicle.
