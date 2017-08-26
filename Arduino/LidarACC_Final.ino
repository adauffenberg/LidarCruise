

#include <LIDARLite.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(69,3); // RX, TX
#include <VirtualWire.h>

// Create a new LIDARLite instance
LIDARLite myLidarLite;

//IMU initialization
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


int distances[120];
int times[120];
int i=0;
int start=0;
int finish=0;
int xlist[120];
int ylist[120];
int positiontrigger=0;
int cruisestate=0;
int loopmax=0;
int objectdis=6000;
int prevobjectdis=0;
int objectwidth=0;
int counter=0;
int reaction=0;
int deltav=0;
byte velocity=0;
byte set=0;
int skip=0;
int bluetoothsend=0;
int incomingByte=0;
int motorcommand=0;
int speedloop=0;
int loopcounter=0;
int ftobjectdis = 0;
int controlincoming=0;

void setup() {
  
  Serial.begin(115200);
  Serial1.begin(19200);
  Serial2.begin(9600);
  Serial3.begin(9600);
  mySerial.begin(9600);
  
  set=0;
  
  
 
  //IMU config
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  
  myLidarLite.begin(0,true);
  myLidarLite.configure(0);
  
}


void loop()
{ 
  //Read photodiode gate level
  if (mySerial.available()) {
    incomingByte = mySerial.read();
    //Serial.print(incomingByte); 
  
   
   //Only start reading process when triggered by serial command of 2 from mirror arduino(read as 50)
  if (incomingByte==50){
     
  
   
    
    /////////////////////////////////////////////LIDAR///////////////////
    
    
    //Adjust this delay until mirror lines up with opening
    //delay(220); //0
    
    start=millis();
    
   
  for(int i = 0; i < 120; i++){
    distances[i]=myLidarLite.distance(false); 
    
    //get time to from start of loop to current measurement
    finish=millis();  
    times[i]=finish-start;
    //when beyond 60 degrees (332 ms) stop gathering lidar points
    if (times[i]>332){
      loopmax=i;
      break;
     
    }
    else{
      loopmax=119;
    }
  }
  
  
  //convert to x,y
    for (i=0;i<loopmax;i++){
      
    if (distances[i]>120){
    xlist[i]=-distances[i]*cos(PI/180*(115-((times[i]-40)*0.18))); //0.18
    ylist[i]=distances[i]*sin(PI/180*(115-((times[i]-40)*0.18)));
    }
    else
    {
      xlist[i]=0;
      ylist[i]=0;
    }
    /*
    Serial.print(xlist[i]);
    Serial.print("\t");
    Serial.print(ylist[i]);
    Serial.println("\t");
    */
    } 
   
    
    //Run once with bias correction, this will be immediately overwritten in the next for loop
    distances[0]=myLidarLite.distance();
    
   
 /////////////////////////////////////////////VSS///////////////////


 Serial1.print(0);
 
   if (Serial1.available()) {
    delay(50);
    velocity = Serial1.read();
   }
   if (set==0){
     set=velocity;
         
   }
  
 
delay(10);

 
 /////////////////////////////////////////////IMU///////////////////
  
  

 Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  //Serial.print("AcX = "); Serial.print(AcX);
  //Serial.print(" | AcY = "); Serial.print(AcY);
  //Serial.print(" | AcZ = "); Serial.print(AcZ);
  //Serial.print(" | Tmp = "); Serial.print(((Tmp/340.00+36.53)*(9/5))+32);  //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  
 
 //IMU Correction of LIDAR 
 //Zero rate value too high, keep for temp reading for debug, IMU correction of lidar around turn not possible
 
 /////////////////////////////////////////////DECISION/SENDING///////////////////
 
 /*
  1 - short press coast
  2 - short press accel
  3 - long press coast
  4 - long press accel
  */
 
  
 prevobjectdis=objectdis;
 objectdis=6000;
 
 objectwidth=0;
 counter=0;
 
 skip=0;
 
  for (i=0;i<(loopmax-1);i++){
    //Find width of object (defn: <3ft (90cm) distance difference between points), if greater than 300cm (10ft) and with 20ft on either side of front, don't react, probably road sloping up into sensor
    if (((ylist[i+1]-ylist[i])<90) && (xlist[i+1]<600) && (xlist[i+1]>-600)) {
      counter=counter+1;
    }
    else {
      if (counter>2) {
        objectwidth=xlist[i]-xlist[i-counter];
        if (objectwidth>300) {
          skip=1;
        }
        counter=0;
      }
      else {
        counter=0;  
      }  
    }
    
    
    //Check each point to see if it is within 152 cm (5ft) of either side of center and less than the previous closest distance
    if ((xlist[i]<152) && (xlist[i]>-152) && (ylist[i]<objectdis) && (ylist[i]>0)) {
      objectdis=ylist[i];
    }
  }
  
  //Serial2.print(" Distance ");
  //Serial2.print(objectdis);
  //Serial2.print(" ");
  
  
  cruisestate=analogRead(A0);
  
  
  deltav=(prevobjectdis-objectdis)*1.5;
  
  //If cruise is not on, do not react
  if (cruisestate>575) {
    reaction=6;
    bluetoothsend=1;
  }
  //If object was determined above to be too wide, don't react
  else if (skip==1) {
    reaction=0;
    skip=0;
    bluetoothsend=2;
  }
  //If deltav > 178 cm/s (4mph), long press coast
  else if (deltav>178) {
    reaction=3;
    bluetoothsend=3;
  }
  //If speed no signal, don't react, (kinda outdated, meant for no GPS, should never enter)
  else if (velocity<0) {
    reaction=0;
    bluetoothsend=13;
  }
  //If velocity is over set speed, short press coast
  else if (velocity>(set+2)) {
    reaction=1;
    bluetoothsend=4;
  }
  //If deltav < 134 cm/s (-3mph) and velocity below 3mph below set, long press accel
  else if ((deltav<-134) && (velocity<(set-3))) {
    reaction=4;
    bluetoothsend=5;  
  }
  //If deltav < 134 cm/s (-3mph) and velocity above 3mph below set, short press accel
  else if ((deltav<-134) && (velocity>=(set-3)) && (velocity<set)) {
    reaction=2;
    bluetoothsend=6;
  }
  else {
    //If distance less than 70ft, long press coast
    if (objectdis<2100) {
      reaction=3;
      bluetoothsend=7;
    }
    //If distance greater than 130 ft and velocity below 3mph below set, long press accel if previous was short press accel, if not short press
    else if ((objectdis>4000) && (velocity<(set-3))) {
      if (reaction==2 || reaction==4){
        reaction=4;
      }
      else {
        reaction=2;
      }
      bluetoothsend=8;
    }
    //If distance greater than 130 ft and velocity above 3mph below set, short press accel
    else if ((objectdis>4000) && (velocity>=(set-3)) && (velocity<set)) {
      reaction=2;
      bluetoothsend=9;
    }
    //If distance greater than 110 ft and deltav less than 0 and previous press was not short press accel, short press accel
    else if ((objectdis>3300) && (deltav<0) && (reaction!=2)) {
      reaction=2;
      bluetoothsend=10;
    }
    //If distance less than 100 ft and deltav greater than 0, short press coast 
    else if ((objectdis<3000) && (deltav>0)) {
      reaction=1;
      bluetoothsend=11;
    }
    //If none of the above, do nothing
    else {
      reaction=0;
      bluetoothsend=12;
    }
  }
      
 //if prev distance =6000 (not in range) change long press coast into short press
 if (prevobjectdis==6000 && reaction==3) {
   reaction=1;
 }
  
 // send reaction to control arduino 
 
 if (reaction<6) {  
 Serial3.print(reaction);
 }
 
 //read from control arduino if available and increase or decrease set speed
 if (Serial3.available()) {
   controlincoming = Serial3.read();
    if (controlincoming == 48) {
      set = set+1;
    }
    else if(controlincoming == 49) {
      set = set -1;
      if (set<0){
        set=0;
      }
    }
    
 }
 
     
 /////////////////////////////////////////////BLUETOOTH///////////////////
  
  Serial2.print("#");
  
  if (set<10){
    Serial2.print("00");
  }
  else {
    Serial2.print(set);
  }
  
  if (velocity<10){
    Serial2.print("00");
  }
  else {
    Serial2.print(velocity);
  }
  
  Serial2.print(reaction);
  
  ftobjectdis=objectdis/30.48;
  Serial2.print(ftobjectdis);
  
  Serial2.print("~");
  
  //Original bluetooth debug code
 /* 
 
 //Serial2.println("test"); 
 if (bluetoothsend==1) {
   Serial2.println("Cruise Off");
 }
 else if (bluetoothsend==2) {
   Serial2.println("Hill: No reaction");
 }
 else if (bluetoothsend==3) {
   Serial2.println("Closing speed >6mph, long press coast");
 }
 else if (bluetoothsend==13) {
   Serial2.println("No GPS");
 }
 else if (bluetoothsend==4) {
   Serial2.println("Greater than set speed, short press coast");
 }
 else if (bluetoothsend==5) {
   Serial2.println("Leaving speed >6mph, >3mph below set, long press accel");
 }
 else if (bluetoothsend==6) {
   Serial2.println("Leaving speed >6mph, <3mph below set, short press accel");
 }
 else if (bluetoothsend==7) {
   Serial2.println("Closer than 40ft, long press coast");
 }
 else if (bluetoothsend==8) {
   Serial2.println("Greater than 130ft, >3mph below set, long press accel");
 }
 else if (bluetoothsend==9) {
   Serial2.println("Greater than 130ft, <3mph below set, short press accel");
 }
 else if (bluetoothsend==10) {
   Serial2.println("Greater than 110ft, deltav<0, short press accel");
 }
 else if (bluetoothsend==11) {
   Serial2.println("Less than 80ft, deltav>0, short press coast");
 }
 else if (bluetoothsend==12) {
   Serial2.println("Else: Do nothing");
 }
 */
 
 //Check and see how long each loop takes
 //Serial.print(millis());
  //  Serial.println(" ");
    
 }////END OF POSITION TRIGGERED LOOP
 }////END OF  SERIAL READ TRIGGERED LOOP
 
 //When in motor fault mode, wait for bluetooth commands to manually rotate motor
 if (incomingByte<50) {
   //Serial.println(incomingByte);
   if (incomingByte==48) {
     Serial2.println("Motor error: Cannot find start position");
   }
   else if (incomingByte==49) {
     Serial2.println("Motor error: Out of range");
   }
   //Look for bluetooth command. If 0 (48) is read, send 0 to mirror arduino to turn left, if 1 (49) is read, send 1 to mirror arduino to turn right
   if (Serial2.available()) {
    motorcommand = Serial2.read();
    if (motorcommand==48) {
      mySerial.print(0);
    }
    else if (motorcommand==49) {
      mySerial.print(1);
    }
   }
 } 
 
 
  
 }
  

