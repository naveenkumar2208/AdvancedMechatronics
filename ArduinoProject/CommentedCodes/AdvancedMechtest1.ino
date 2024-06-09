#include <AdvancedMech.h> //including custom library

//defining all the pins
#define ledIntrPin 13
#define ledObjPin 11
#define ledObjDetPin A5
#define leftServoPin 12
#define rightServoPin 10
#define irFarLeftPin 4
#define irFarRightPin 5
#define irMiddleLeftPin 2
#define irMiddleRightPin 3
#define triggerPinFront 8
#define echoPinFront 9
#define triggerPinSide 6
#define echoPinSide 7


completeTask bot(ledIntrPin, ledObjPin, ledObjDetPin, leftServoPin, rightServoPin, irFarLeftPin, irFarRightPin, irMiddleLeftPin, irMiddleRightPin, triggerPinFront, echoPinFront, triggerPinSide, echoPinSide); //the main class object called bot with the conscructor
void setup() {
  Serial.begin(9600); //serial begin statement
  bot.init(); //bot intitalization function for servos
   
} 

void loop() {
  bot.task(); //main task having all inbuilt functions
}
