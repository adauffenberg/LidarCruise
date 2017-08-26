float buttonread = 0;
float buttonvoltage = 0;
byte incomingByte = 0;
byte prevByte = 0;
unsigned long commandexp = 0;
unsigned long buttonexp = 0;
int lidarsend = 2;
#include <SoftwareSerial.h>

SoftwareSerial mySerial(A5,A4); // RX, TX

void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode (5, OUTPUT); 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  //Click on/off button to turn off, seems to come on everytime car started
  delay(200);
  digitalWrite(7, HIGH);
  
}

void loop() {
  delay(100);
  buttonread=analogRead(1);
  
  buttonvoltage=(1/((5*buttonread/1024)/(5-(5*buttonread/1024))))*1000;
  
  
  /*
  1 - short press coast
  2 - short press accel
  3 - long press coast
  4 - long press accel
  */
  
  
  if (mySerial.available()) {
    prevByte = incomingByte;
    incomingByte = mySerial.read();
    if (incomingByte==48) {
      incomingByte=0;
    }
    else if (incomingByte==49) {
      incomingByte=1;
    }
    else if (incomingByte==50) {
      incomingByte=2;
    }
    else if (incomingByte==51) {
      incomingByte=3;
    }
    else if (incomingByte==52) {
      incomingByte=4;
    }
    //stay in serial mode for .8 seconds
    commandexp= millis()+800;
    //check if long or short press, set up button expiration accordingly
      if ((incomingByte>0) && (incomingByte<3)) {
        buttonexp=millis()+100;
      }
      else if ((incomingByte>2) && (incomingByte<5)) {
        buttonexp=millis()+800;
      }
     
     if (lidarsend<2){ 
     mySerial.print(lidarsend);
     lidarsend = 2;
     }
  }
  
  
  
  //If no serial data for .8 second, passthrough button presses //NON SERIAL MODE
  if (commandexp<millis()) {  
  //Serial.println("Non Serial Mode");  
  //On/Off
   if ((buttonvoltage>0) && (buttonvoltage<35)){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    }
  //Cancel
  else if((buttonvoltage>115) && (buttonvoltage<175)){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  //Set/coast
  else if((buttonvoltage>690) && (buttonvoltage<750)){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  //Res/Accel
  else if((buttonvoltage>2050) && (buttonvoltage<2500)){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
  else{
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  }
  //If serial data, passthrough on/off and cancel, ignore other two buttons; for 1.1 seconds //SERIAL MODE
else if (commandexp>millis()) { 
 //Serial.println("Serial Mode"); 
  //On/Off
   if ((buttonvoltage>0) && (buttonvoltage<35)){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    //If button is pressed stop current command until new serial data is received
    
    commandexp=0;
    }
  //Cancel
  else if((buttonvoltage>115) && (buttonvoltage<175)){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    //If button is pressed stop current command until new serial data is received
    //Prevents cancel button from being followed by another button press since main arduino takes 1 second to realize "Cruise" light is not on
    
    commandexp=0;
  }
  //Set/coast
  else if (((incomingByte==1) || (incomingByte==3)) && (buttonexp>millis())){
    
    //if previously pressing accel, write nothing and wait 100 millis to prevent overlap of relays
    if ( prevByte == 2 || prevByte == 4) { 
    prevByte = incomingByte;   
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    delay(100);
    }
    
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    
  }
  //Res/Accel
  else if(((incomingByte==2) || (incomingByte==4)) && (buttonexp>millis())){
    //if previously pressing coast, write nothing and wait 100 millis to prevent overlap of relays
    
    if ( prevByte == 1 || prevByte == 3) {  
    prevByte = incomingByte;  // Change prevByte so only does once, prevents flutter of relay
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    delay(100);    
    }

    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    
  }
  else{
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
   
  }
  
  //Set/coast
  if((buttonvoltage>690) && (buttonvoltage<750)){
    lidarsend=1;
  }
  //Res/Accel
  else if((buttonvoltage>2050) && (buttonvoltage<2500)){
    lidarsend=0;
  }
  
  
  
}
}
