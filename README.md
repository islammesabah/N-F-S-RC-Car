# N-F-S-RC-Car

## ● Brief description:

Our project aims to build an RC Car with some safety and entertainment
features which were already applied to modern cars that made our lives
better; however, safety features are more crucial to our lives and have
higher priorities compared to entertainment features so we will make it with
higher priority.

We will implement three features in our project:

### 1. Lane Departure Warning (LDW):

  In this feature we alert the driver by using LED and Buzzer about a potential threat when his vehicle drifts out of the road (lane).
  
### 2. Warning Indicators and Power Mirrors (WIPM):

  In our second feature we will collect some information about the car and show it to the user on LCD. These information include raining or not, seatbelt fastened or not, fuel levels in the car, and the state of our smart mirrors by using joystick and servo motor to control and simulate our smart mirrors.
  
### 3. Sound System:
  
  We used MP3 module to build this system by using SD card which contains some mp3 files that is controlled by a push button which play/pause files; Furthermore, We swipe between files back and forth using button.

**We use FreeRTOS to assign a priority for every feature to make the workflow and the car reliable.**

## ● The components:

| # | Name | #components | function |
| - | - | - | - |
| 1 | Arduino Mega | 1 | The main component that have the code, takes input, and assigns output to the sensors and actuators. |
| 2 | Arduino cable | 1 | To connect Arduino board to the table and load the code to it.|
| 3 | Battery | 3 | 3v to supply motor by power. |
| 4 | AC-DC Adaptor | 1 | Supply our board and sensors by power. |
| 5 | Line Follower Sensor | 2 | To know if the car follows its line or not. |
| 6 | Buzzer | 1 | Warn the user when seatbelt is not fastened or when the car drifts away from its lane. |
| 7 | Red Led | 1 | Warn the user when the car is not on the line. |
| 8 | Breadboard | 1 | Connect all components together and supply them with power. |
| 9 | Car body | 1 | The Structure of our car |
| 10 | DC motor with wheel | 4 | To move our car. |
| 11 | H-bridge | 1 | Controls DC motor with Arduino. |
| 12 | Rain-Drop sensor | 1 | To know if it is raining or not. |
| 13 | Water level sensor | 1 | Estimates fuel level in the car. |
| 14 | Joystick | 1 | Controls the adjustable mirror. |
| 15 | Servo motor | 1 | Simulate the adjustable mirror. |
| 16 | LCD | 1 | Displays information about all WIPM features. |
| 17 | Variable resistor | 1 | Adjust LCD brightness. |
| 18 | MP3 module | 1 | Taking files from the SD card and control its play (play/ pause, next/previous). |
| 19 | SD card | 1 | Storage for MP3 files. |
| 20 | Small speaker | 1 | Plays sound and makes it audible. |
| 21 | Switch button | 1 | Controls the power of H-bridge. |
| 22 | Push buttons | 6 | Controls MP3 module and simulate the seatbelt. 1 button for play/pause music, 1 for play next, 1 for play previous, 2 for volume (option), 1 for simulate seatbelt. |
| 23 | 10k Ohm Resistor | 6 | Connecting push buttons. |
| 24 | 330 Ohm Resistor | 1 | Connecting LEDs. |
| 25 | Male-male jumpers | A lot | Connect all components together. |
| 26 | Male-female jumpers | A | lot Connect all components together. |

## ● The full circuit:

![The full circuit](https://github.com/islamkaloop/N-F-S-RC-Car/blob/dev/full%20circuit.png)

## ● Libraries:

  I. Arduino_FreeRTOS: Library used to make our code run according to certain schedule and according to assign priority to every task.

  II. Semphr: This library uses semaphores in our project to prevent next and previous buttons from working when the state of playing the MP3 file is paused.

  III. LiquidCrystal: To use LCD.

  IV. Servo: To make servo motor work.

  V. SoftwareSerial: For controlling the DFPlayer (MP3 module).

## ● Inputs:

We take inputs from several modules and buttons we collect all of them in
this table:

| # | Name | # pins we use | Type of input |
| - | -| -| - |
| 1 | Line follower | 1 | Digital (we put it in A0 and A1 but we read digital as they are closer to the sensors) on (A0, A1). |
| 2 | Joystick | 3 | (Digital) on (A5), (VRy, VRx) (Analog) on (A6, A7).|
| 3 | Rain RainDrop sensor | 2 | We use only the analog pin (Analog) on (A14).|
| 4 | Water level | 1 | Analog on (A15). |
| 5 | Bush buttons| 6 | Digital on (5, 6, 16, 17, 18, 19). |
| 6 | Bluetooth module | 2 | Rx, Tx (Serial communication (USART) and TTL compatible) on (14, 15). |

## ● Outputs:

We used a lot of actuators from this table:

| # |Name | # pins we use | Type of output. |
| - | -| -| - |
| 1 | H-bridge module | 2 for enable/4 for controlling the direction. | 2-enable (PWM) on (44, 46), 4(Digital) on (47, 49, 51, 53). |
| 2 | Servo motor | 1 | PWM on (45). |
| 3 | Buzzer | 1 | Digital on (7). |
| 4 | LCD | 6 | Digital on (13, 12, 11, 10, 9, 8). |
| 6 | DFPlayer (Mp3 module) | 2 | UART on (4, 3). |

## ● Prioritization and Division of the tasks:

We divide our tasks into eight tasks:

| # | Name | Priority | Delay | Usage | Note |
| - | -| -| - | - | - |
| 1 LineFollower | 1 | 10ms | Reads line follower sensor input and warns the user. | - |
| 2 MoveCar | 1 | 10ms | Reads from Bluetooth module and moves the car. | - |
| 3 WaterLevel | 1 | 2000ms | Reads fuel level and redisplays on the LCD. | - |
| 4 RainDrop | 1 | 2000ms | Reads when it rains and redisplays on the LCD. | - |
| 5 ServoAndJoyStick | 1 | 1000ms | Take control of mirror and print its state on LCD. | - |
| 6 Seatbelt | 1 | 1000ms | We Check if the seatbelt is connected and print its state on LCD. | - |
| 7 LCDDisplay | 0 | -- | Displays current state on LCD. | At first the priority of LCD is 0, then when a tsk wants to use it, its priority reach maximum (2) to finish first, then go back to 0 again.|
| 8 | MP3 | 1 | 800ms | Controls MP3 module.| - |

## ● Problems and Limitations:

We faced some problems and limitations when we implemented our project. We collected them in the following points:

  o The car’s physical structure was small; As a result, putting on a lot of
components and connecting them was messy.

  o As we used a lot of components, which in turn use a lot of power, we
couldn’t make all features work without connecting them with multi
sources of power; indeed, we used 9V battery and voltage adapter.

  o As we connected the DC Motor which moved the car with only 9v, it
couldn’t handle different speeds so it moved only when we set it
with high speed.

  o There was some delay in the push button functionality when it is
triggered, this was the result of the scheduler waiting to finish the
task before it.
