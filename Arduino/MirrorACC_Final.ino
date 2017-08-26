#include <Stepper.h>
#include <SoftwareSerial.h>

const int stepsPerRevolution = 200;  

// initialize the stepper library on pins 3 through 6:
Stepper myStepper(stepsPerRevolution, 3, 4, 5, 6);
int positionmain=0;
int positionerror=0;
int counter=0;
int incomingByte=0;

SoftwareSerial mySerial(9, 10); // RX, TX

void setup() {
  
 
// set the speed at 60 rpm:
  mySerial.begin(9600);
  Serial.begin(9600);
  myStepper.setSpeed(30);
  positionmain=analogRead(A3);
  positionerror=analogRead(A4);

  //Rotate right to left until position sensor is triggered or more than 35 steps, (initialize start position)
    while(positionmain>10 && counter<36 && positionerror>10) {
      myStepper.step(-1);
      positionmain=analogRead(A3);
      positionerror=analogRead(A4);
      //Serial.println(positionmain);
      //Serial.println(positionerror);
      //Serial.println(counter);
      counter=counter+1;
      }
     //if start position not found, write 0 to serial for "cannot find start error" and wait for command from serial to manually move left or right
     if (counter>35) {
       Serial.println("initialize start error");
       mySerial.print(0);
       while (counter>35) {
         incomingByte=0;
         if (mySerial.available()) {
           incomingByte = mySerial.read();
         }
         //If 0 is sent, move left 18 degrees
         if (incomingByte==48) {
           myStepper.step(-10);
         }
         //If 1 is sent, move right
         else if (incomingByte==49) {
           myStepper.step(10);
         }
       }
     }
   
     //if error position gate (180 degrees around) is triggered, write 1 to serial for "out of range error" and wait for command from serial to manually move left or right
     else if (positionerror<11) {
       Serial.println("initialize out of range error");
       mySerial.print(1);
       while (positionerror<11) {
         incomingByte=0;
         if (mySerial.available()) {
           incomingByte = mySerial.read();
         }
         //If 0 is sent, move left
         if (incomingByte==48) {
           myStepper.step(-10);
         }
         //If 1 is sent, move right
         else if (incomingByte==49) {
           myStepper.step(10);
         }
       }
     }
     
     counter=1;
     delay(200);
}


void loop() {
  // step 60 degrees right:
  myStepper.step(33);
  //if never entered loop to step left (counter=0), through error wait for manual movement
  if (counter==0) {
    Serial.println("no steps to the left error");
       mySerial.print(0);
       while (counter==0) {
         incomingByte=0;
         if (mySerial.available()) {
           incomingByte = mySerial.read();
         }
         //If 0 is sent, move left 18 degrees
         if (incomingByte==48) {
           myStepper.step(-10);
         }
         //If 1 is sent, move right
         else if (incomingByte==49) {
           myStepper.step(10);
         }
       }
     }
   counter=0;
  // step left until 
  positionmain=analogRead(A3);
  positionerror=analogRead(A4);
  //Serial.println(positionmain);
  //Serial.println(counter);
  //Serial.println(positionerror);
 while (positionmain>10 && counter<20 && positionerror>10) {
    myStepper.step(-2);
      positionmain=analogRead(A3);
      positionerror=analogRead(A4);
      //Serial.println(positionmain);
      //Serial.println(positionerror);
      //Serial.println(" ");
      counter=counter+1;
  }
  //if start position not found, write 0 to serial for "error" and wait for command from serial to manually move left or right  
    if (counter>19) {
      Serial.println("start error");
       mySerial.print(0);
       while (counter>19) {
         incomingByte=0;
         if (mySerial.available()) {
           incomingByte = mySerial.read();
         }
         //If 0 is sent, move left 18 degrees
         if (incomingByte==48) {
           myStepper.step(-10);
         }
         //If 1 is sent, move right
         else if (incomingByte==49) {
           myStepper.step(10);
         }
       }
     }
    //if error position gate (180 degrees around) is triggered, write 1 to serial for "error" and wait for command from serial to manually move left or right 
    else if (positionerror<11) {
     Serial.println("out of range error"); 
     mySerial.print(1);
     while (positionerror<11) {
         incomingByte=0;
         if (mySerial.available()) {
           incomingByte = mySerial.read();
         }
         //If 0 is sent, move left
         if (incomingByte==48) {
           myStepper.step(-10);
         }
         //If 1 is sent, move right
         else if (incomingByte==49) {
           myStepper.step(10);
         }
     }
   }
   
   //If not two above error must have exited while loop due to position main trigger, send command to lidar arduino
   else if (positionmain<11) {
     mySerial.print(2);
     //Serial.println("sent");
  
}
   
}
  
  

  

