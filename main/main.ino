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
int motorBenable = 46;
int motorApin0 = 53;
int motorApin1 = 51;
int motorBpin0 = 49;
int motorBpin1 = 47;     
char state;
int vSpeed=125;
//WaterLevel
int waterLevel = A15;
//RainDrop
int rainDrop = A14;
//servo And JoyStick
Servo servo_right; 
Servo servo_left;   
const int SW_pin = A5;
const int X_pin = A7; 
const int Y_pin = A6; 
const int RightSe = 45;
const int LeftSe = 2;
//SeatBelt
int seatBelt = 16;
int seatBeltConnected = 0; 
//lcd             
LiquidCrystal lcd(13,12,11,10,9,8); 
String wear="        ";
String waterlevelString="        ";
String raining="        ";
String mirrorString="        ";
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
int BuzzerUsed = 0;

// define Tasks
//Cat 1 
void LineFollower( void *pvParameters); 
void MoveCar( void *pvParameters);
//Cat 2
void WaterLevel( void *pvParameters); 
void RainDrop( void *pvParameters);
void ServoAndJoyStick( void *pvParameters); 
void Seatbelt( void *pvParameters);
void LCDDisplay( void *pvParameters); 
//Cat 3
void MP3( void *pvParameters);


TaskHandle_t xTask2Handle = NULL;

SemaphoreHandle_t SoundSem = NULL;
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
 // Serial.setTimeout(10);
  //WaterLevel
  pinMode(waterLevel,INPUT);
  //RainDrop
  pinMode(rainDrop,INPUT);
  //servo And JoyStick
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  servo_right.attach(RightSe);  
  servo_left.attach(Left4Se);
  servo_right.write(90);                
  servo_left.write(90); 
  mirrorString = "Open";
  //SeatBelt
  pinMode(seatBelt,INPUT);
  digitalWrite(seatBelt,HIGH);
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
  setVolume(vol);
  playFirst();
  isPlaying = true;
  //common
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer,LOW);
  Serial.begin(9600);
  Serial.println("hi");

  SoundSem = xSemaphoreCreateMutex();
  if ( ( SoundSem ) != NULL )
     xSemaphoreGive( SoundSem );
  //create Tasks
  xTaskCreate (LineFollower, "LineFollower", 256, NULL, 1, NULL);
  xTaskCreate (MoveCar, "MoveCar", 256, NULL, 1,NULL);
  xTaskCreate (WaterLevel, "WaterLevel", 256, NULL, 1, NULL);
  xTaskCreate (RainDrop, "RainDrop", 256, NULL, 1,NULL);
  xTaskCreate (ServoAndJoyStick, "ServoAndJoyStick", 256, NULL, 1, NULL);
  xTaskCreate (Seatbelt, "Seatbelt", 256, NULL, 1,NULL);
  xTaskCreate (LCDDisplay, "LCDDisplay", 256, NULL, 1, &xTask2Handle);
  xTaskCreate (MP3, "LCDDisplay", 256, NULL, 1,NULL);
  vTaskStartScheduler();
}

void loop() {
// Empty. Things are done in Tasks.
}

void LineFollower (void *pvParameters) 
{
  while (1) {
     // Serial.println("hi1");

      if(!digitalRead(leaftsensor) && !digitalRead(rightSensor))  
      {
          if(BuzzerUsed != 1)
            digitalWrite(Buzzer,LOW);
          digitalWrite(led,LOW);
      }
    
       if(digitalRead(leaftsensor) && digitalRead(rightSensor))    
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH); 
          BuzzerUsed = 2;
      }
      
      if(!(digitalRead(leaftsensor)) && digitalRead(rightSensor)) 
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH);  
          BuzzerUsed = 2;
       }
      
      if(digitalRead(leaftsensor) && !(digitalRead(rightSensor)))  
      {
          digitalWrite(Buzzer,HIGH);
          digitalWrite(led,HIGH);  
          BuzzerUsed = 2;
      }
      vTaskDelay ( pdMS_TO_TICKS( 10 ) ) ;
  }
} 
void MoveCar( void *pvParameters)  
{
  while (1) {
      //Serial.println("hi2");
     if(Serial3.available()){
        state = Serial3.read();
        Serial.println(state);
      }
      int s = 0;
      switch(state){
        case '0': vSpeed=10;s=1;break;
        case '1': vSpeed=25;s=1;break;
        case '2': vSpeed=51;s=1;break;
        case '3': vSpeed=77;s=1;break;
        case '4': vSpeed=102;s=1;break;
        case '5': vSpeed=128;s=1;break;
        case '6': vSpeed=153;s=1;break;
        case '7': vSpeed=179;s=1;break;
        case '8': vSpeed=204;s=1;break;
        case '9': vSpeed=230;s=1;break;
        case 'q': vSpeed=255;s=1;break;
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
      if(s==1){
      analogWrite(motorAenable, vSpeed);
      analogWrite(motorBenable, vSpeed);
      }
    //  Serial.println("hisss");
      vTaskDelay ( pdMS_TO_TICKS( 10 ) ) ;
      //delay(5);
  }
    
 }

void WaterLevel( void *pvParameters) 
{
  while (1) {
      //Serial.println("hi3");

     int waterlevelValue = analogRead(waterLevel); 
  
      if (waterlevelValue<=480){ 
        waterlevelString="0 mm"; 
        //lcd.clear();
      }
      else if (waterlevelValue>480 && waterlevelValue<=530){ 
        waterlevelString="0-5 mm"; 
        //lcd.clear();
      }
      else if (waterlevelValue>530 && waterlevelValue<=615){ 
        waterlevelString="5-10 mm"; 
        //lcd.clear();
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
      vTaskPrioritySet( xTask2Handle, 2 );
      vTaskDelay ( pdMS_TO_TICKS( 2000 ) ) ;
   }
 }
 void RainDrop (void *pvParameters)
{
  while (1) {
     //  Serial.println("hi4");

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
        //lcd.clear();
        break;
     }
     vTaskPrioritySet( xTask2Handle, 2 );
      vTaskDelay ( pdMS_TO_TICKS( 2000 ) ) ;
  }
}

void ServoAndJoyStick( void *pvParameters) 
{
  while (1) {
    int y =  analogRead(Y_pin);
      Serial.println(y);

     if(y>800)
      {
        servo_right.write(90);                
        servo_left.write(90); 
        mirrorString = "Open";
      }
      
      if(y<200)
      {                                 
        servo_right.write(0);                
        servo_left.write(180); 
        mirrorString = "Close";
      }
      Serial.println(y);
      vTaskPrioritySet( xTask2Handle, 2 );
      vTaskDelay ( pdMS_TO_TICKS( 1000 ) ) ;
   }     
 }
 void Seatbelt (void *pvParameters) 
{
  while (1) {
     // Serial.println("hi6");

    if(digitalRead(seatBelt)==LOW)
     {
      if(seatBeltConnected==0)
        seatBeltConnected = 1;
      else 
        seatBeltConnected = 0;
     } 
      if(seatBeltConnected==0)
      {
        digitalWrite(Buzzer,HIGH);
        BuzzerUsed = 1;
        wear = "Not Wear";
      }
      else
      { 
        if(BuzzerUsed != 2)
          digitalWrite(Buzzer,LOW);
        wear = "Wear";
        //lcd.clear();
      } 
      vTaskPrioritySet( xTask2Handle, 2 );
      vTaskDelay ( pdMS_TO_TICKS( 1000 ) ) ;
   }
}

void LCDDisplay( void *pvParameters)
{
  while (1) {
    lcd.clear();
    Serial.println("hi7");
    lcd.setCursor(0,0); 
    lcd.print(wear);
    lcd.setCursor(0,1); 
    lcd.print(waterlevelString);
    lcd.setCursor(9,0); 
    lcd.print(raining); 
    lcd.setCursor(9,1); 
    lcd.print(mirrorString); 
    //vTaskDelay ( pdMS_TO_TICKS( 1000 ) ) ;
    vTaskPrioritySet( xTask2Handle, 0 );
  }
    
 }
 void MP3 (void *pvParameters) 
{
  while (1) {
     // Serial.println("hi8");

     if (digitalRead(buttonVolInc) == LOW)
      {
        if(vol<42){
          vol=vol+5;
          setVolume(vol);
        }
      }

     if (digitalRead(buttonVolDec) == LOW)
       {
        if(vol>5){
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
          xSemaphoreTake( SoundSem, portMAX_DELAY);
        }else
        {
          isPlaying = true;
          play();
          xSemaphoreGive(SoundSem);
        }
      }
     if (digitalRead(buttonNext) == LOW)
      {
        if(isPlaying)
        {
          xSemaphoreTake( SoundSem, portMAX_DELAY);
          playNext();
          xSemaphoreGive(SoundSem);
        }
      }
       if (digitalRead(buttonPrevious) == LOW)
      {
        if(isPlaying)
        {
          xSemaphoreTake( SoundSem, portMAX_DELAY);
          playPrevious();
          xSemaphoreGive(SoundSem);
        }
      }
      vTaskDelay ( pdMS_TO_TICKS( 500 ) ) ;
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

