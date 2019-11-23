#include <Arduino_FreeRTOS.h> 
#include <semphr.h>
#include <LiquidCrystal.h>
#include <Servo.h> 
#include "SoftwareSerial.h"

//define vairables
//line follower
int leaftsensor = A1;
int rightSensor = A0;     
int led  = 21; 
//Move Car
int motorAenable = 44;
int motorBenable = 44;
int motorApin0 = 53;
int motorApin1 = 51;
int motorBpin0 = 49;
int motorBpin1 = 47;     
char state;
int vSpeed=50;
//WaterLevel
int waterLevel = A12;
//RainDrop
int rainDrop = A8;
//servo And JoyStick
Servo servo_right; 
Servo servo_left;   
const int SW_pin = A5;
const int X_pin = A7; 
const int Y_pin = A6; 
const int RightSe = 20;
const int LeftSe = 2;
//SeatBuilt
int seatBuilt = 16;
int seatBuiltConnected = 0; 
//lcd             
LiquidCrystal lcd(13,12,11,10,9,8); 
String wear;
String waterlevelString;
String raining;
String mirrorString="Open";
//MP3
SoftwareSerial mySerial(4,3);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 
int buttonNext = 17;
int buttonVolInc=5;
int buttonVolDec=6;
int buttonPause = 18;
int buttonPrevious = 19;    
int vol=24;
boolean isPlaying = false;
//common
int Buzzer = 7;

// define Tasks
//Cat 1 
void LineFollower( void *pvParameters); 
void MoveCar( void *pvParameters);
//Cat 2
void WaterLevel( void *pvParameters); 
void RainDrop( void *pvParameters);
void ServoAndJoyStick( void *pvParameters); 
void Seatbuilt( void *pvParameters);
void LCDDisplay( void *pvParameters); 
//Cat 3
void MP3( void *pvParameters);

void setup() {
  ///pin assign
  //line follower
  pinMode(leaftsensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
  //Move Car
  pinMode(motorAenable,OUTPUT);   
  pinMode(motorBenable,OUTPUT);   
  pinMode(motorApin0,OUTPUT);   
  pinMode(motorApin1,OUTPUT);   
  pinMode(motorBpin0,OUTPUT);   
  pinMode(motorBpin1,OUTPUT);
  Serial3.begin(9600);
  Serial3.setTimeout(10);
  //WaterLevel
  pinMode(waterLevel,INPUT);
  //RainDrop
  pinMode(rainDrop,INPUT);
  //servo And JoyStick
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  servo_right.attach(RightSe);  
  servo_left.attach(LeftSe);
  servo_right.write(90);          
  servo_left.write(90); 
  //SeatBuilt
  pinMode(seatBuilt,INPUT);
  digitalWrite(seatBuilt,HIGH);
  //lcd 
  lcd.begin(16,2);
  //MP3
  pinMode(buttonPause, INPUT);
  digitalWrite(buttonPause,HIGH);
  pinMode(buttonNext, INPUT);
  digitalWrite(buttonNext,HIGH);
  pinMode(buttonPrevious, INPUT);
  digitalWrite(buttonPrevious,HIGH);
  pinMode(buttonVolInc, INPUT);
  digitalWrite(buttonVolInc,HIGH);
  pinMode(buttonVolDec, INPUT);
  digitalWrite(buttonVolDec,HIGH);
  mySerial.begin (9600);
  delay(1000);
  setVolume(vol);
  playFirst();
  isPlaying = true;
  //common
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer,LOW);
  Serial.begin(9600);
  Serial.println("hi");
  //create Tasks
  xTaskCreate (LineFollower, "LineFollower", 256, NULL, 1, NULL);
  xTaskCreate (MoveCar, "MoveCar", 256, NULL, 1,NULL);
  xTaskCreate (WaterLevel, "WaterLevel", 256, NULL, 1, NULL);
  xTaskCreate (RainDrop, "RainDrop", 256, NULL, 1,NULL);
  xTaskCreate (ServoAndJoyStick, "ServoAndJoyStick", 256, NULL, 1, NULL);
  xTaskCreate (Seatbuilt, "Seatbuilt", 256, NULL, 1,NULL);
  xTaskCreate (LCDDisplay, "LCDDisplay", 256, NULL, 1, NULL);
  xTaskCreate (MP3, "LCDDisplay", 256, NULL, 1,NULL);
}

void loop() {
// Empty. Things are done in Tasks.
}

void LineFollower (void *pvParameters) 
{
  while (1) {
      Serial.println("hi1");

      if(!digitalRead(leaftsensor) && !digitalRead(rightSensor))  
      {
          digitalWrite(Buzzer,LOW);
          digitalWrite(led,LOW);
      }
    
       if(digitalRead(leaftsensor) && digitalRead(rightSensor))    
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH); 
      }
      
      if(!(digitalRead(leaftsensor)) && digitalRead(rightSensor)) 
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH);  
       }
      
      if(digitalRead(leaftsensor) && !(digitalRead(rightSensor)))  
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH);  
      }
      delay(1000);
  }
}

void MoveCar( void *pvParameters)  
{
  while (1) {
      Serial.println("hi2");

     if(Serial3.available()){
        state = Serial3.read();
        Serial.println(state);
      }
      
      switch(state){
        case '0': vSpeed=10;break;
        case '1': vSpeed=25;break;
        case '2': vSpeed=51;break;
        case '3': vSpeed=77;break;
        case '4': vSpeed=102;break;
        case '5': vSpeed=128;break;
        case '6': vSpeed=153;break;
        case '7': vSpeed=179;break;
        case '8': vSpeed=204;break;
        case '9': vSpeed=230;break;
        case 'q': vSpeed=255;break;
        case 'F': digitalWrite(motorApin1,HIGH);
                  digitalWrite(49,HIGH);
                  break;
        case 'B': digitalWrite(motorApin0,HIGH);
                  digitalWrite(motorBpin1,HIGH);
                  break;
        case 'R': digitalWrite(motorBpin0,HIGH);break;
        case 'L': digitalWrite(motorApin1,HIGH);break;
        case 'S': digitalWrite(motorApin0,LOW);
                  digitalWrite(motorApin1,LOW);
                  digitalWrite(motorBpin0,LOW);
                  digitalWrite(motorBpin1,LOW);
                  break;
      }
      analogWrite(44, vSpeed);
      analogWrite(46, vSpeed);
      delay(1000);
  }
    
 }

void WaterLevel( void *pvParameters) 
{
  while (1) {
      Serial.println("hi3");

     int waterlevelValue = analogRead(waterLevel); 
  
      if (waterlevelValue<=480){ 
        waterlevelString="0 mm"; 
        lcd.clear();
      }
      else if (waterlevelValue>480 && waterlevelValue<=530){ 
        waterlevelString="0-5 mm"; 
        lcd.clear();
      }
      else if (waterlevelValue>530 && waterlevelValue<=615){ 
        waterlevelString="5-10 mm"; 
        lcd.clear();
      }
      else if (waterlevelValue>615 && waterlevelValue<=660){ 
        waterlevelString="10-15 mm"; 
      } 
      else if (waterlevelValue>660 && waterlevelValue<=680){ 
        waterlevelString="15-20 mm"; 
      }
      else if (waterlevelValue>680 && waterlevelValue<=690){ 
        waterlevelString="20-25 mm"; 
      }
      else if (waterlevelValue>690 && waterlevelValue<=700){ 
        waterlevelString="25-30 mm"; 
      }
      else if (waterlevelValue>700 && waterlevelValue<=705){ 
        waterlevelString="30-35 mm"; 
      }
      else if (waterlevelValue>705){ 
        waterlevelString="35-40 mm"; 
      }
      delay(1000);
   }
 }
 void RainDrop (void *pvParameters)
{
  while (1) {
       Serial.println("hi4");

    int sensorReading = analogRead(rainDrop);
    int range = map(sensorReading, 0, 1024, 0, 3);
    switch (range) {
      case 0:    
        raining="Raining";
        break;
      case 1: 
        raining="Raining";
        break;
      case 2:   
        raining="Clear";
        lcd.clear();
        break;
     }
    delay(1000);
  }
}

void ServoAndJoyStick( void *pvParameters) 
{
  while (1) {
      Serial.println("hi5");

     if(analogRead(Y_pin)>800)
      {
        servo_right.write(90);                
        servo_left.write(90); 
        mirrorString = "Open";
      }
      
      if(analogRead(Y_pin)<200)
      {                                 
        servo_right.write(0);                
        servo_left.write(180); 
        mirrorString = "Close";
      }
      delay(1000);
   }     
 }
 void Seatbuilt (void *pvParameters) 
{
  while (1) {
      Serial.println("hi6");

    if(digitalRead(seatBuilt)==LOW)
     {
      if(seatBuiltConnected==0)
        seatBuiltConnected = 1;
      else 
        seatBuiltConnected = 0;
     } 
      if(seatBuiltConnected==0)
      {
        digitalWrite(Buzzer,HIGH);
        wear = "Not Wear";
      }
      else
      { 
        digitalWrite(Buzzer,LOW);
        wear = "Wear";
        lcd.clear();
      } 
       delay(1000);
   }
}

void LCDDisplay( void *pvParameters)
{
  while (1) {
      Serial.println("hi7");

    lcd.setCursor(0,0); 
    lcd.print(wear);
    lcd.setCursor(0,1); 
    lcd.print(waterlevelString);
    lcd.setCursor(9,0); 
    lcd.print(raining); 
    lcd.setCursor(9,1); 
    lcd.print(mirrorString); 
    delay(1000);
  }
    
 }
 void MP3 (void *pvParameters) 
{
  while (1) {
      Serial.println("hi8");

     if (digitalRead(buttonVolInc) == LOW)
      {
        if(vol<48){
          vol=vol+5;
          setVolume(vol);
        }
      }

     if (digitalRead(buttonVolDec) == LOW)
       {
        if(vol>0){
          vol=vol-5;
          setVolume(vol);
        }
        }
      
     if (digitalRead(buttonPause) == LOW)
      {
        if(isPlaying)
        {
          pause();
          isPlaying = false;
        }else
        {
          isPlaying = true;
          play();
        }
      }
     if (digitalRead(buttonNext) == LOW)
      {
        if(isPlaying)
        {
          playNext();
        }
      }
       if (digitalRead(buttonPrevious) == LOW)
      {
        if(isPlaying)
        {
          playPrevious();
        }
      }
    delay(1000);
  }
}


//////////////////MP3 Functions
void playFirst()
{
      execute_CMD(0x3F, 0, 0);
      delay(500);
      setVolume(20);
      delay(500);
      execute_CMD(0x11,0,1); 
      delay(500);
}
void pause()
{
      execute_CMD(0x0E,0,0);
      delay(500);
}
void play()
{
      execute_CMD(0x0D,0,1); 
      delay(500);
}
void playNext()
{
      execute_CMD(0x01,0,1);
      delay(500);
}
void playPrevious()
{
      execute_CMD(0x02,0,1);
      delay(500);
}
void setVolume(int volume)
{
      execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
      delay(2000);
}
void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
    // Calculate the checksum (2 bytes)
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    // Build the command line
    byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
    //Send the command line to the module
    for (byte k=0; k<10; k++)
    {
    mySerial.write( Command_line[k]);
    }
}

