#include "Arduino.h" //including ardunio library 
#include "AdvancedMech.h" //including custom(advanced mechatronics) library
#include "Servo.h" //including servo library

Servo ML, MR; //global servo objects
int _pin, _left, _right, _farLeft, _middleLeft, _middleRight, _farRight, _triggerPin1, _echoPin1, _triggerPin2, _echoPin2, _value; //global private variable

static int _count = 0; //static variable for intersection count

completeTask::completeTask(int ledIntrPin, int ledObjPin, int ledObjDetPin, int leftServoPin, int rightServoPin, int irFarLeftPin, int irFarRightPin, int irMiddleLeftPin, int irMiddleRightPin, int triggerPin1, int echoPin1, int triggerPin2, int echoPin2){ //making the constructor
	Serial.begin(9600); //serial.begin statement for debugging in computer
	pinMode(ledIntrPin, OUTPUT); //pin mode of intersection led set to output
	pinMode(ledObjPin, OUTPUT); //pin mode of object detection led set to output
	pinMode(ledObjDetPin, OUTPUT); //pin mode of object led for 5 cm stop led set to output
	pinMode(irFarLeftPin, INPUT); //pin mode of far left IR set to input
	pinMode(irFarRightPin, INPUT); //pin mode of far right IR set to input
	pinMode(irMiddleLeftPin, INPUT); //pin mode of middle left IR set to input
	pinMode(irMiddleRightPin, INPUT); //pin mode of middle IR set to input
	pinMode(triggerPin1, OUTPUT); //pin mode of front Ultrasonic's trigger set to output
	pinMode(echoPin1, INPUT); //pin mode of front Ultrasonic's echo set to input
	pinMode(triggerPin2, OUTPUT); //pin mode of side Ultrasonic's trigger set to output
	pinMode(echoPin2, INPUT); //pin mode of side Ultrasonic's echo set to input
	_ledIntrPin = ledIntrPin; //setting the private variable
	_ledObjPin = ledObjPin; //setting the private variable
	_ledObjDetPin = ledObjDetPin; //setting the private variable
	_leftServoPin = leftServoPin; //setting the private variable
	_rightServoPin = rightServoPin; //setting the private variable
	_irFarLeftPin = irFarLeftPin; //setting the private variable
	_irFarRightPin = irFarRightPin; //setting the private variable
	_irMiddleLeftPin = irMiddleLeftPin; //setting the private variable
	_irMiddleRightPin = irMiddleRightPin; //setting the private variable
	_triggerPin1 = triggerPin1; //setting the private variable
	_echoPin1 = echoPin1; //setting the private variable
	_triggerPin2 = triggerPin2; //setting the private variable
	_echoPin2 = echoPin2; //setting the private variable
}

void completeTask::init(){ //initialization function
	ML.attach(_leftServoPin); //attaching left servo
	MR.attach(_rightServoPin); //attaching right servo
	ML.write(750); //writing left servo to stationary
	MR.write(750); //writing right servo to stationary
}

void completeTask::ledOn(int pin){ //led on function
	digitalWrite(pin, HIGH); //digital write of pin of parameter in function as high
	_pin = pin; //setting the private variable
}

void completeTask::ledOff(int pin){ //led off function
	digitalWrite(pin, LOW); //digital write of pin of parameter in function as high
	_pin = pin; //setting the private variable
}

void completeTask::botStationary(){ //bot stationary function
	ML.write(750); //writing left servo values to middle(750)
	MR.write(750); //writing right servo values to middle(750)
}
void completeTask::botForward(){ //bot forward function
	ML.write(800); //writing left servo values to 800
	MR.write(700); //writing right servo values to 700
}
void completeTask::botBackward(){ //bot backward function
	ML.write(700); //writing left servo values to 700
	MR.write(800); //writing right servo values to 800
}

void completeTask::botTurnRight(){ //bot turn right function
	ML.write(800); //writing left servo values to 800
	MR.write(750); //writing right servo values to 750
}

void completeTask::botTurnLeft(){ //bot turn left function
	ML.write(750); //writing left servo values to 750
	MR.write(700); //writing right servo values to 700
}

void completeTask::botRotateLeft(){ //bot rotates around its z axis (counter clockwise)
	ML.write(700); //writing left servo values to 700
	MR.write(700); //writing right servo values to 700
}

void completeTask::botRotateRight(){ //bot rotates around its z axis (clockwise)
	ML.write(800); //writing the left servo to 800
	MR.write(800); //writing the right servo to 800
}


int completeTask::irReadFLeft(){ //reading the value of far left IR
	_irReading = digitalRead(_irFarLeftPin); //digital read of specified ir pin
	return _irReading; //return statement
}

int completeTask::irReadFRight(){ //reading the value of far right specified IR
	_irReading = digitalRead(_irFarRightPin); //digital read of ir pin
	return _irReading; //return statement
}

int completeTask::irReadMLeft(){ //reading the value of middle left  specifiedIR
	_irReading = digitalRead(_irMiddleLeftPin); //digital read of ir pin
	return _irReading; //return statement
}

int completeTask::irReadMRight(){ //reading the value of middle right specified IR
	_irReading = digitalRead(_irMiddleRightPin); //digital read of ir pin
	return _irReading; //return statement
}

int completeTask::readUSF(){ //reading values of front ultrasonic sensor (distance)
	digitalWrite(_triggerPin1, LOW); //used the example given in the Arduino website
	delayMicroseconds(2);
	digitalWrite(_triggerPin1, HIGH);
	delayMicroseconds(10);
	digitalWrite(_triggerPin1, LOW);
	_duration = pulseIn(_echoPin1, HIGH);
	_distance = _duration * 0.034 / 2;
	return _distance; //return statement
}

int completeTask::readUSSs(){ //reading values of side ultrasonic sensor (distance)
	digitalWrite(_triggerPin2, LOW); //used the example given in the Arduino website
	delayMicroseconds(2);
	digitalWrite(_triggerPin2, HIGH);
	delayMicroseconds(10);
	digitalWrite(_triggerPin2, LOW);
	_duration = pulseIn(_echoPin2, HIGH);
	_distance = _duration * 0.034 / 2;
	return _distance; //return statement
}

void completeTask::lineFollow(){ //line follow function
	if((irReadMLeft() == 1) && (irReadMRight() == 1)){ //reading both middle IR values and seeing if it is 1
		botForward(); //bot forward function called here
	}
	else if((irReadMLeft() == 1) && (irReadMRight() == 0)){ //reading both middle IR values and seeing if it is 1 and 0 for middle left and middle right respectively 
		botTurnLeft(); //bot turn left function called here
	}
	else if((irReadMLeft() == 0) && (irReadMRight() == 1)){ //reading both middle IR values and seeing if it is 0 and 1 for middle left and middle right respectively 
		botTurnRight(); //bot turn right function called here
	}
	else{ // else
		botStationary(); //bot stationary function called here
	}
}

int completeTask::intersection(){ //checking if there is an intersection
	if((irReadFLeft() == 1) && (irReadFRight() == 1)){ //reading both middle IR values and seeing if it is 1
		ledOn(_ledIntrPin); //led on function 
		
		return 1; //return statement
	}
	
	else { //else statement
		ledOff(_ledIntrPin); //led off function 
		return 0; //return statement
	}
}


void completeTask::botAutonomousRotation(){ //bot automatic rotation to left function
	while(irReadMRight() == 0){ //if middle right IR is 0 it will execute the following
		botRotateLeft(); //keep rotating left
	}
	
}


void completeTask::botRotate(){ //bot rotation function
	botBackward(); //bot backward to fix rotation about the axis by giving a very small translation along -ve y-axis
	delay(340); //a large delay statement 
	botStationary(); //bot stationary command
	delay(500); //a very large delay statement
	Serial.println("rotate"); //for debugging printing 
	botRotateLeft(); //force rotate left
	delay(600); //a very large delay statement
	botAutonomousRotation(); //function called to intimate rotation
	botStationary(); //bot stationary command
	delay(100); //a moderate delay statement 
}



void completeTask::objectDetection(){ //obeject detection function
	if ( intersection() == 1 && readUSSs() >10 && readUSSs() <40 ){ //checking if there is an intersection and checking if object is within 10 to 40 cm of side ultrasonic sensor(robot)
		ledOn(_ledObjPin); //turning on led
		delay(500); //large delay statement
		ledOff(_ledObjPin); ///turning off led
		botRotate(); //bot rotate function
		delay(500); // a large delay
		while(readUSF()>3){ //if object detected in the front ultrasonic sensor is placed at a distance of greater than 3 cm
			lineFollow(); //line follow
		}
		botStationary(); //bot stationary command
		ledOn(_ledObjDetPin); //led on Statement
		delay(600); //a large delay statement 
		ledOff(_ledObjDetPin); //led off statement
		botRotate(); //bot rotate function
		delay(1000); //a very large delay statement
		Serial.println("Test4"); //for debugging printing 
		while(intersection() == 0){ // if there is not intersection it will execute the following
		Serial.println(intersection()); //debugging statement 
		lineFollow(); //line follow statement
		}
		botStationary(); //bot stationary command
		botRotate(); //bot rotate function called
		delay(1500); //a very large delay statement
		delay(70); //a very small delay statement for correction
		while(intersection()==0){ // to cross the previous intersection 
			lineFollow(); //line follow command
			delay(60); //a small delay amplified by while loop
			
		}
		while(intersection() == 1){ // to cross the previous intersection 
			lineFollow(); //line follow command
			delay(60); //a small delay amplified by while loop
		}
		
		botStationary(); //bot stationary command
		delay(2000); //a very large delay
		}
		else if(intersection() == 1 && readUSSs() >40 ){ //else if no object
			while(intersection() == 1){ //to skip the current intersection
				lineFollow(); //line follow
				delay(30); //a small delay amplified by while loop
			}
			botStationary(); //bot stationary command
			delay(5000); //an extremely large delay
			
		}


}

int completeTask::intersectionCount(){ //intersection counter
	static boolean truth = false; //static object in function and setting it to false
	if(intersection() == 1 && truth == false){ // to avoid recounting the same intersection
		_count++; //adding the counter
	}
	truth = intersection(); //finding if already at an intersection and to avoid multiple counts
	return _count; //return statement
}

int completeTask::trueObjDet(){ //check if object is present left of robot from the specified range 
	if (readUSSs() >= 10 && readUSSs() <= 40 ){
		return 1; //return statement
	}
	else return 0; //return statement

}

void completeTask::frontObjTask(){ //front object task
	if(frontObjDet() == 1) //if front object detection is 1 it will execute
	{
		ledOn(_ledObjPin); //led on for specified pin
		delay(500); //a large delay
		ledOff(_ledObjPin); //led off for specified pin
		while(readUSF()>3){ //infinite loop for front Ultrasonic sensor if distance is grater than 3
			lineFollow(); //line follow function
			Serial.println(readUSF()); //debugging statement
		}
		ledOn(_ledObjDetPin); //led on for specified pin
		botStationary(); //bot stationary command
		delay(500); //a large delay
		ledOff(_ledObjDetPin); //led off for specified pin
		botRotate(); //bot rotate function (180 rotation)
		delay(500); // a large delay
		botStationary(); //bot stationary command
		lineFollow(); //line follow command
	}
	else {
		Serial.println("0"); //debugging statement while testing
		lineFollow(); //line follow function
		delay(1000); //a very large delay
		botRotate(); //bot rotate function
		delay(500); // a lage delay
		botStationary(); //bot stationary command
		delay(500); //a large delay value
	}
}

int completeTask::frontObjDet(){ //front object detection function
	if(readUSF() >5 && readUSF()<20){ //if distance is between 5 and 20 cm
		return 1; //return statement
	}
	else { //else statement
		return 0; //return statement
	}
}


void completeTask::task(){ //main function fusion of everything above
	volatile int x; //volatile int value for rapid counter
	if(intersection() == 1){ //checks if there is an intersection
		x = intersectionCount(); //gets the intersection count value
		switch(x){ //switch case
				case 1:
				case 12:
					botStationary();
					delay(250);
					while(intersection()==1){
						lineFollow();
					}
					break; //for both case 1 and 12 which are the start positions of the robot it will line follow while going and coming back from home position
				case 2:
				case 3:
				case 4:
				case 5:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
					botStationary();
					delay(250);
					objectDetection();
					break; //for other intersection object detection function
				case 6:
					botStationary();
					delay(1000);
					objectDetection();
					frontObjTask();
					break; //for last intersection while going it will execute object detection plus front object detection
				default:
					botStationary();
					delay(1000);
					while(intersection()==1){
						lineFollow();
					}
					break; //default is line follow
		}
		Serial.println(intersectionCount()); //debugging statement
		
	}
	else{ //if there is not intersection
		lineFollow(); //line follow
	}
}






