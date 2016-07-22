//importing values and initializing variables
#include <Servo.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
Servo myservo;
Servo myservotwo;


enum Groups
{
  Group0 =0,
 
  Wlist1 = 1,
  Wlist2 = 2,
  Wlist3 = 3,
  
};

enum Group0
{
  Robot = 0,
  TESTER = 1,
};
enum Wlist1 
{
  
  ACTION =0,
  MOVE = 1,
  TURN =2,
  RUN =3,
  LOOK =4,
  ATTACK =5,
  STOP =6,
  HELLO =7,
};
enum Wlist2
{
  LEFT =0,
  RIGHT =1,
  UP =2,
  DOWN =3,
  FORWARD =4,
  BACKWARD =5,
};
enum Wlist3
{
  ZERO =0,
  ONE =1,
  TWO =2,
  THREE =3,
  FOUR=4,
  FIVE =5,
  SIX =6,
  SEVEN=7,
  EIGHT =8,
  NINE=9,
  TEN  =10,
};
int8_t group =0, idx = -1;
// Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
#include "SoftwareSerial.h"
SoftwareSerial port(12, 13);

#define pcSerial SERIAL_PORT_MONITOR

int servo1speed =90, servo2speed =90, timemotorspin=0;
#include "EasyVR.h"
String commandused;
EasyVR easyvr(port);
void setup() {
  //pinMode(13, OUTPUT);
  //connecting serial monitor
  pcSerial.begin(9600);
  port.begin(9600);
  //detect if easyvr is connected 
  if (!easyvr.detect())
  { Serial.println("EasyVR not detected!");}
  else {
    easyvr.setPinOutput(EasyVR::IO1, LOW);
    Serial.println("EasyVR detected");
  }
  easyvr.setTimeout(5);
  easyvr.setLanguage(0);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
 
  



  group = 0; //<-- trigger word group

}
void loop() {
  
  
  //if voice commands have been acitvated and servos are supposed to spin 
  if (timemotorspin > 0)
  {
    
    
    if (servo1speed != 90 || servo2speed !=90)
    {
      
    
      
      if (servo1speed != 90)
      {
        myservo.attach(A0);
      }
      if (servo2speed != 90)
      {
        myservotwo.attach(A1);
      }
      myservo.write(servo1speed);
      myservotwo.write(servo2speed);
      delay(timemotorspin);
      myservo.detach();
      myservotwo.detach();
      timemotorspin = 0;
      servo1speed = 90;
      servo2speed = 90;
    }
  }
  //reseting servospeed in case servo speed has changed w/o time
  servo1speed = 90;
  servo2speed = 90;
  //
  easyvr.setPinOutput(EasyVR::IO1, LOW); 
  
 //starting the voice recog part
 // put your main code here, to run repeatedly:
 
 Serial.print("Say a command in group");
 Serial.print(group);
//easyvr voice rec
easyvr.recognizeWord(group);

 easyvr.recognizeCommand(group);

 //when commmand has been recieved
 while (!easyvr.hasFinished());
    //digitalWrite(13, LOW);
 
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED off
 
  
   //finds place where
   idx = easyvr.getCommand();
  
   idx = easyvr.getWord();

   //if a command has been recieved find it and do stuff accordingly
   if (idx >= 0){
    
     Serial.print(group + "____" + idx);
     action();
     //since group has most likely been changed change led number
     if (group == Wlist1)
     {
      digitalWrite(11, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
     }
     else if (group == Wlist2)
     {
      digitalWrite(11, LOW);
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
     }
     else if (group == Wlist3)
     {
      digitalWrite(11, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
     }
     idx = -1;
   }
  
}



//depeding on keyword find it with a switch case statement and do stuff accordingly
void action()
{
  
  switch (group)
  {
    case Group0:
      
      switch (idx)
      {
        case Robot:
          //group = Group1;
          group = Wlist1;
          Serial.println("Robot");
          break;
       
          
      }
      break;
  
    case Wlist1:
      switch (idx)
        {
          case ACTION:
          
            commandused = "action";
            group = Wlist2;
            Serial.println("Action");
            break;
          case MOVE:
            group =Wlist3;
            Serial.println("Move");
            commandused = "action";
            break;
          case TURN:
            commandused = "turn";
            group = Wlist2;
            Serial.println("Turn");
            break;
          case RUN:
            Serial.println("Run");
            servo1speed = 0;
            servo2speed = 180;
            timemotorspin = 895;
            break;
        }
        break;
    case Wlist2:
     
      switch (idx)
      {
        case LEFT:
          if (commandused == "turn")
          {
            group = Wlist1;
            Serial.println("Left");
            servo1speed = 0;
            timemotorspin =950;
            commandused = "";
            
          }
          else
          {
            Serial.println("Invalid command");
          }
          break;
        case UP:
          group = Wlist1;
          break;
        case RIGHT:
          if (commandused == "turn")
          {
            group = Wlist1;
            Serial.println("Right");
            servo2speed = 180;
            timemotorspin = 850;
            commandused = "";
          }
          else
          {
            Serial.println("Invalid command");
          }
          break;
        case FORWARD:
          if (commandused == "action" && timemotorspin != 0)
          {
            group = Wlist1;
            Serial.println("FORWARD");
            servo1speed = 0;
            servo2speed = 180;
            Serial.println(timemotorspin/895);
            commandused = "";
          }
          else if (timemotorspin ==0 && commandused == "action")
              {timemotorspin = 895;
               commandused = "";
               servo1speed = 0;
               servo2speed = 180;
               group = Wlist1;
              Serial.println("Testing activated");
              }
          
          else 
          {
            Serial.println("Invalid command");
          }
          break;
        case BACKWARD:
          if (commandused == "action" && timemotorspin !=0)
          {
            group = Wlist1;
            Serial.println("BACKWARD");
            servo1speed = 180;
            servo2speed = 0;
            Serial.println(timemotorspin/895);
            commandused = "";
          }
          else if (timemotorspin ==0 && commandused == "action")
          { 
            group = Wlist1;
            timemotorspin = 895;
            servo1speed = 180;
            servo2speed = 0;
          
            
            Serial.println("Testing activated");
          }
          else  
          {
            Serial.println("Invalid command");
          }
          break;
      }
      break;
    case Wlist3:
      if (idx>=1){
        timemotorspin = 895 * idx;
        group = Wlist2;   
        Serial.println("Number entered"); 
        Serial.println(idx);
        Serial.println(timemotorspin);
      }
      else if (idx ==0)
      {
        group = Wlist1;
      }
       
       break;
      
    break;
  }
}

