#include <VirtualWire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10); // RX, TX



long count;
byte mph;
long mphsum;
int x;
int incomingByte = 0;

void setup() {
  mySerial.begin(19200);
  Serial.begin(115200);
  pinMode(5,INPUT);
}

void loop() {
  
  
  //10 sample average, reject if less than 1
  x=0;
  mphsum=0;
  for( int i=0; i<5; i++){
  count=pulseIn(5,HIGH,40000);
  //Serial.println(count);
  if (count<40000 && count>3000){
  mphsum=(450000/count)+mphsum;
  x=x+1;
  }
  }
   
  //Serial.println(mphsum); 
  if (mphsum>0){
    mph=mphsum/x;
  }
  else {
    mph=0;
  }
  
    //mySerial.print(57);
  //Wait until 0 is sent by LIDAR ACC arduino, then send mph over serial
  if (mySerial.available()) {
    delay(100);    
       
    if (mySerial.read()==48) {
    mySerial.write(mph);   
    /*
    Serial.print(mph);
    Serial.print(" ");
    Serial.print(millis()); 
    Serial.println(" ");
    */
    }
  }

  }
