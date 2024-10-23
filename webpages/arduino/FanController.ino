// https://www.xsimulator.net/community/threads/simtools-v3-3d-wind-dash-example-with-arduino-sketch.17963/

// Arduino code to achieve wind simulation with the Adafruit Motor Shield V2 instead of the Monster Moto
// Based on the work of sallerding, Zed, SilentChill, Avenga76, and others with inspiration from noorbeast
// Supports right,left and traditional serial speed output from GameDash (R000, L000, S000)
// Adafruit Motor Shield V2 library is required to be added to Arduino IDE interface prior to uploading sketch
// Please wire your two fans to motor ports M1 and M3 on the adafruit Motor Shield V2 (Not M1 and M2)
// This will utilize both mosfets on the board and reduce the potential for overheating with 1.3 Amp fans

// Version Date 01_28_2023

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define FALSE 0
#define TRUE 1
#define BOTH 0
#define LEFT 1
#define RIGHT 2

// Define a fan idlespeed here if you prefer to have your fans on all the time for ventilation
// A default idlespeed is set so that you know the sketch is working, set to 0 for no fans at idle
#define Idlespeed 20

int Speed = 0;
int bufferArray[4];
int whichFan = BOTH;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Set port M1 for Motor 1 (Left fan)
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
// Set port M3 for Motor 3 (Right fan)
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);

void setup()
{
AFMS.begin(); // Initialize the shield
Serial.begin(9600); // Initialize serial communication

// Initialize motors at idlespeed
Motor1->setSpeed(Idlespeed);
Motor1->run(FORWARD);
Motor3->setSpeed(Idlespeed);
Motor3->run(FORWARD);
}

void loop()
{
ReadData();

if (whichFan == BOTH)
{
Motor1->setSpeed(Speed);
Motor3->setSpeed(Speed);
Motor1->run(FORWARD);
Motor3->run(FORWARD);
if (Speed == 0)
{
Motor1->run(RELEASE); // Release the motor
Motor3->run(RELEASE); // Release the motor
}
}

if (whichFan == LEFT)
{
Motor1->setSpeed(Speed);
Motor1->run(FORWARD);
if (Speed == 0)
{
Motor1->run(RELEASE); // Release the motor
}
}

if (whichFan == RIGHT)
{
Motor3->setSpeed(Speed);
Motor3->run(FORWARD);
if (Speed == 0)
{
Motor3->run(RELEASE); // Release the motor
}
}
}

void ReadData()
{
// We need 4 characters - the command followed by three digits

bool haveCommand = FALSE;
bool haveStart = FALSE;

while (haveCommand == FALSE) // can't exit until have a full valid cddd command
{
// where c is a valid char and ddd is a valid 3
// character representation of three digits

// Valid command always starts with an S (legacy for both fans), L (left fan), or R (right fan)
while (haveStart == FALSE)
{
while (Serial.available() == 0); // spin and wait for data
bufferArray[0] = Serial.read(); // have data, read it

if (bufferArray[0] == 'S') //S
{
whichFan = BOTH;
haveStart = TRUE;
}
else if (bufferArray[0] == 'L') //L
{
whichFan = LEFT;
haveStart = TRUE;
}
else if (bufferArray[0] == 'R') //R
{
whichFan = RIGHT;
haveStart = TRUE;
}
}

// Now need the numbers - will just read three and throw them away if any don't qualify
// if we unsynchronize, it will fail valid digits and go back to waiting for command char
for (int i = 1; i < 4; i++) // read and label each byte.
{
while (Serial.available() == 0); // spin and wait for each character to arrive
bufferArray = Serial.read(); // store as they come in
}

// Check the numbers for validity
if (isDigit(bufferArray[1]) && isDigit(bufferArray[2]) && isDigit(bufferArray[3]))
// all are numbers - have a full cddd command
haveCommand = TRUE; // otherwise start over looking for a new and valid command
}
// Now turn that into a number and clip to 255 - rely on Game Dash for proper scaling
Speed = ((bufferArray[1]-48)*100) + ((bufferArray[2]-48)*10) + ((bufferArray[3]-48)*1);
//Serial.println(SpeedGameDash);
if (Speed > 255)
Speed = 255;
// Add logic for idle speed defined above by user
if (Speed < Idlespeed)
Speed = Idlespeed;
}