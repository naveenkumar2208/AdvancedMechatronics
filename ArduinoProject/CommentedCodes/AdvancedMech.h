#ifndef AdvancedMech_h //if header is already included ignore the rest
#define AdvancedMech_h //define AdvancedMech.h

#include "Arduino.h" //including the ardunio.h library
#include "Servo.h" // including the servo.h library


class completeTask { //defining the class name
	public: //defining the public members
		completeTask(int ledIntrPin, int ledObjPin, int ledObjDetPin, int leftServoPin, int rightServoPin, int irFarLeftPin, int irFarRightPin, int irMiddleLeftPin, int irMiddleRightPin, int triggerPin1, int echoPin1, int triggerPin2, int echoPin2); // main constructor
		void ledOn(int pin); //led on fuction
		void ledOff(int pin); //led off function
		void init(); //initialization function for the class
		void botStationary(); //bot will stationary function
		void botForward(); //bot will move forward function
		void botBackward(); //bot will move backward function
		void botTurnRight(); //bot will turn right function
		void botTurnLeft(); //bot will turn left function
		void botRotateLeft(); //bot will rotate left function
		void botRotateRight(); //bot will rotate right function
		int irReadFLeft(); //reading the left most(far) IR function
		int irReadFRight(); //reading the right most(far) IR function
		int irReadMLeft(); //reading the middle left IR function
		int irReadMRight(); //reading the middle right IR function
		int readUSF(); //reading the front Ultrasonic sensor function
		int readUSSs(); //reading the side Ultrasonic sensor function
		void lineFollow(); //line follow function
		int intersection(); //intersection detection function
		void objectDetection(); //object detection function
		int intersectionCount(); //intersection counter function
		void task(); //main task function for whole operation
		void botAutonomousRotation(); //rotations will engage automatically to the left function
		void botRotate(); //complete rotation function fusing the previous function by dislodging the movement a bit
		int trueObjDet(); //check for object detection
		int counterValue(); //intersection counter value
		int frontObjDet(); //front obj detection function
		void frontObjTask(); //front object task function 
			
	
	private: //defining the private members
		int _ledIntrPin; //pin for led indicator for intersection
		int _ledObjPin; //pin for led indicator for object
		int _ledObjDetPin; //pin for led indicator for 5cm before object
		int _pin; //pin for led on and off functions
		int _leftServoPin; //pin for left servo
		int _rightServoPin; //pin for right servo
		int _irFarLeftPin; //pin for far left IR
		int _irFarRightPin; //pin for far right IR
		int _irMiddleLeftPin; //pin for middle left IR
		int _irMiddleRightPin; //pin for middle right IR
		int _triggerPin1; //trigger pin for front Ultrasonic sensor
		int _echoPin1; //echo pin for frontUltrasonic sensor
		int _triggerPin2; //trigger pin for side Ultrasonic sensor
		int _echoPin2; //echo pin for side Ultrasonic sensor
		Servo ML, MR; //servo objects
		long _duration; // int variable for duration in ultrasonic
		int _distance; // int variable for distance in ultrasonic
		int _irReading; //int variable IR readings
		
		
};
#endif
