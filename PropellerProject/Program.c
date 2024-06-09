// ALL INCLUDE STATEMENTS

#include "servodiffdrive.h"
#include "simpletools.h"

//---------------------------------------------------------------------------------------------------------------------------

//ALL COGS

int *cog1; //cog 1 reference
int *cog2; //cog 2 reference
int *cog3; //cog 3 reference
int *cog4; //cog 4 reference
int *cog5; //cog 5 reference
int *cog6; //cog 6 reference
int *cog7; //cog 7 reference

//---------------------------------------------------------------------------------------------------------------------------

//ALL STACKS



unsigned int stack1[40+25]; //stack1 value
unsigned int stack2[40+25]; //stack2 value
unsigned int stack3[40+25]; //stack3 value
unsigned int stack4[40+25]; //stack4 value
unsigned int stack5[40+25]; //stack5 value
unsigned int stack6[40+25]; //stack6 value
unsigned int stack7[40+25]; //stack7 value


//---------------------------------------------------------------------------------------------------------------------------

// PINS


#define pinIrMiddleLeft 1 //ir middle left pin
#define pinIrMiddleRight 0 //ir middle right pin
#define pinIrLeft 2 //ir right pin
#define pinIrRight 3 //ir right pin
#define frontTrigPin 14 // trigger Pin of front ultrasonic sensor
#define frontEchoPin 15 // echo Pin of front ultrasonic sensor
#define leftServoPin 12 //left Servo Pin
#define rightServoPin 13 //right Servo Pin
#define pinIntersectionIndicator 9 //pin for LCD
#define sideTrigPin 6 //trigger Pin of side(left) ultrasonic sensor
#define sideEchoPin 7 //trigger Pin of side(left) ultrasonic sensor
#define ledPin 10 //led pin for object detectiions(goals)
#define object 11 //object detection led


//---------------------------------------------------------------------------------------------------------------------------

//GLOBAL VARIABLES

volatile unsigned int counter = 0; // global counter variable
unsigned int locationOfJam; //global int storing location of traffic jam
unsigned int totalIntersections = 0; //global int for storing all visited intersections so far
unsigned int goalsDetected = 0; //global int for storing how many goal locations it has seen

//---------------------------------------------------------------------------------------------------------------------------

// FORWARD DELARATIONS OF FUNCTIONS

void ledOn(const unsigned int pin); //forward declaration of function to turn on led
void ledOff(const unsigned int pin); //foward declaration of function to turn off led
int IR(const unsigned int pin); //forward declaration of function to get IR input
int distance(const unsigned int trigPin, const unsigned int echoPin); //forward declaration of function to find distance from ultrasonic sensor
void servoInit(const unsigned int leftpin, const unsigned int rightpin); //forward declaration of function to attach pins
void forward(); //forward declaration of move forward function
void backward(); //forward declaration of move backward function
void left(); //forward declaration of  move left function
void right(); //forward declaration of  move right function
void invLeft(); //forward declaration of inverse move left function
void invRight(); //forward declaration of inverse move right function
void stop(); //forward declaration of  stationary function
void lineFollow(); //forward declaration of line following function
void invlinFollow(); //forward declaration of inverse line following function
int intersection(); //forward declaration of detecting an intersection
int intersectionCount(); //forward declaration of intersection counter
int frontObj(); //forward declaration of function to see if there is a front object
void rotateLeft(); // forward declaration of function to rotate the bot left
void rotateRight(); //forward declaration of function to rotate the bot right
void detectJamAndReturn(); //forward declaration of function to find where Jam is and come back to start 
void autoRotateLeft(); //forward declaration of function to rotate left till it finds black line
void autoRotateRight(); //forward declaration of function to rotate right till it finds black line
void followPath(); //forward declaration of function to follow path set which is got to i1-B1-B2-B3-B4-i4-A4-A3-A2-A1-i1-B1-B2-B3-B4-B5
void goalLocations(); //forward declaration of function to get how many goal locations it has seen so far
void goalLocationsTest(); //forward declaration of function to get how many goal locations it has seen so far


//---------------------------------------------------------------------------------------------------------------------------

// MAIN LOOP


int main()
{  
  servoInit(leftServoPin, rightServoPin);  
  //detectJamAndReturn();
  cog1 = cogstart((void*)detectJamAndReturn, NULL, stack1, sizeof(stack1));  
  //goalLocationsTest();

}


//---------------------------------------------------------------------------------------------------------------------------

// ALL FUNCTIONS

//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TURN ON LED

void ledOn(const unsigned int pin) //function to turn on led
{
  high(pin); //high pin
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TURN OFF LED

void ledOff(const unsigned int pin) //function to turn off led
{
  low(pin); // low pin
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DISPLAY LCD


//--------------------------------------------------------------------------------------------------------------------------- 
//FUNCTION TO TAKE IR READINGS

int IR(const unsigned int pin) //function to get IR input
{
  volatile int i = input(pin); //volatile int storing input of pin
  //print("%d\n", i); //printing value for testing and calibration
  return i; //returns the IR sensor reading
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO RECORD DISTANCE FRO ULTRASONIC SENSOR
int distance(const unsigned int trigPin, const unsigned int echoPin) //function to get distance from ultrasonic sensor
{
  low(trigPin); // setting trig Pin to low
  pulse_out(trigPin, 10); //pulse out of trigger Pin
  volatile long tEcho = pulse_in(echoPin, 1); //pulse in of echo Pin
  volatile int cmDist = tEcho / 58; //getting distance
  //print("%d cm\n", cmDist); //printing distance for testing
  return cmDist; //returning distance
  
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO INTIALISE SERVO
void servoInit(const unsigned int leftpin, const unsigned int rightpin) //function to attach servos
{
  drive_pins(leftpin, rightpin); //attaching servos
  drive_stop(); //stopping the servos
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DRIVE FORWARD
void forward() //drive forward
{
  drive_speeds(-200,-200); //full forward
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DRIVE BACKWARD
void backward() //drive backward
{
  drive_speeds(200,200); //full backward
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TILT LEFT
void left() //turn left
{
  drive_speeds(-200, 0); //keeping left wheel stationary
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TILT RIGHT
void right() //turn right
{
  drive_speeds(0, -200); //keeping right wheel stationary
} 
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR INV LEFT 
void invLeft() //inverse move left function
{
 drive_speeds(200, 0); //keeping left wheel stationary 
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR INV RIGHT
void invRight() //inverse move left function
{
 drive_speeds(0, 200); //keeping left wheel stationary 
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR STOPPING
void stop() //bot stationary
{
  drive_stop(); //stops the servos 
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO ROTATE RIGHT
void rotateRight() //bot rotate right
{
  drive_speeds(200, -200); //rotating both wheels same speed in opposite direction to make it rotate right 
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO ROATE LEFT
void rotateLeft() //bot rotate left
{
  drive_speeds(-200, 200); //rotating both wheels same speed in opposite direction to make it rotate left
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR AUTO ROTATION TO LEFT
void autoRotateLeft() //function to rotate left till it finds black line
{
  while(IR(pinIrMiddleRight) == 1) //while loop to get out of black line
  {
    rotateLeft(); //rotate left
  }
  stop(); //stop motors
  while(IR(pinIrMiddleRight) == 0) // while loop to get back into black line
  {
    rotateLeft(); //rotate left
  }
  stop(); //stop motors  
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR AUTO ROTATION RIGHT
void autoRotateRight() //function to rotate right till it finds black line
{
  while(IR(pinIrMiddleLeft) == 1) //while loop to get out of black line
  {
    rotateRight(); //rotate right
  }
  stop(); //stop motors  
  while(IR(pinIrMiddleLeft) == 0) // while loop to get back into black line
  {
    rotateRight(); //rotate right
  }
  stop(); //stop motors    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR LINE FOLLOW  

void lineFollow() //line follow function
{
  if (IR(pinIrMiddleLeft) == 1 && IR(pinIrMiddleRight) == 1) //if both middle IR read 1 it will move forward
  {
    forward(); //calling forward function
  }
  else if (IR(pinIrMiddleLeft) == 1 && IR(pinIrMiddleRight) == 0) //if left IR reads 1 and right IR reads 0
  {
    left(); //turn left
  }
  else if (IR(pinIrMiddleLeft) == 0 && IR(pinIrMiddleRight) == 1) //if left IR reads 0 and right IR reads 1
  {
    right(); //turn right
  }
  else //any other condition
  {
    stop(); //stop
  }    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR INV LINE FOLLOWING
void invlineFollow() //line follow function
{
  if (IR(pinIrMiddleLeft) == 1 && IR(pinIrMiddleRight) == 1) //if both middle IR read 1 it will move forward
  {
    backward(); //calling forward function
  }
  else if (IR(pinIrMiddleLeft) == 1 && IR(pinIrMiddleRight) == 0) //if left IR reads 1 and right IR reads 0
  {
    invRight(); //turn left
  }
  else if (IR(pinIrMiddleLeft) == 0 && IR(pinIrMiddleRight) == 1) //if left IR reads 0 and right IR reads 1
  {
    invLeft(); //turn right
  }
  else //any other condition
  {
    stop(); //stop
  }    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR INTERSECTION DETECTION

int intersection() //function to detect if there is an intersection
{
  if (IR(pinIrLeft) == 1 && IR(pinIrRight) == 1) //if both far IR's read 1
  {
    //print("1 \n"); //printing value for testing
    ledOn(pinIntersectionIndicator);
    return 1; //return 1
  }
  else //else 
  {
    ledOff(pinIntersectionIndicator);
    //print("0 \n"); //printing value for testing
    return 0; //return 0
  }    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR COUNTING INTERSECTION  

int intersectionCount() //function to count the intersection
{
  
  static unsigned int truth = 0; //fake object to avoid multiple counts
  
  if (intersection() == 1 && truth == 0) //checking if there is an intersection and avoid recounts
  {
    counter++; //incrementing the counter
  }    
  truth = intersection(); //setting the fake object to see if it still at an intersection
  //print("number of intersections so far = %d\n" ,counter); //printing value for testing
  totalIntersections = totalIntersections + counter;
  //print("Total intersections so far = %d\n", totalIntersections); //printing total intersections so far for testing
  return counter; //returning the count
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DETECT WHERE THE JAM IS AT RETURN ITS LOCATION
int frontObj() //function to see if there is an object in front
{
  unsigned int trafficAt = intersectionCount(); 
  if (distance(frontTrigPin, frontEchoPin) <= 7) //checking if distance is less than or equal to 5
  {
    //stop(); //stop the bot
    //print("Traffic jam at %d\n", trafficAt +1); //printing for testing
    ledOn(ledPin);
    locationOfJam = trafficAt + 1; //setting global variable to store where Jam is located
    return trafficAt + 1; //return the location of the jam
  }    
  else //else statement
  {
    //lineFollow(); //line following till jam is detected
    return 0; //returns 0 if false
  }    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO START FRO HOME AND COME BACK TO I0 AFTER DETECTING WHERE THE JAM IS AT
void detectJamAndReturn() //function to find where Jam is and come back to start 
{
  while((frontObj() != 3) && (frontObj() != 4) && (frontObj() != 6) ) //while loop to ensure line follow keep executing till condition not satisfied
  {
    lineFollow(); //line follow
  }    
  counter = 0; //resetting the counter
  for (int i =0; i<=50;i++) //for loop
  {
    stop(); //stopping the bot for smooth motion 
  }    
  autoRotateLeft(); //180 degree rotation
  ledOff(ledPin);
  switch (locationOfJam) //switch case for the where the jam is at
  {
    case 3 : //case 3
      while(intersectionCount() != 1) //till count = 2 it will keep following the line
      {
        lineFollow(); //line follow
      }
      for (int i =0; i<=50;i++) //for loop
      {
        stop(); //stopping the bot for smooth motion 
      }
      break; //break statement
    case 4 : //case 4
      while(intersectionCount() != 2) //till count = 3 it will keep following the line
      {
        lineFollow(); //line follow
      }
      for (int i =0; i<=50;i++) //for loop
      {
        stop();  //stopping the bot for smooth motion 
      }
      break; //break statement
    case 6 : //case 6
      while(intersectionCount() != 4) //till count = 5 it will keep following the line
      {
        lineFollow(); //line follow 
      }
      for (int i =0; i<=50;i++) //for loop
      {
        stop(); //stopping the bot for smooth motion 
      }
      break;  //break statement     
  }
  

  stop();  //stopping the bot for smooth motion  
  autoRotateRight(); //90 degree rotation 
  for (int i =0; i<=250;i++) //for loop
  {
    backward(); //going back
  }
  stop(); //stop
  counter = 0; //resetting the counter
  followPath();
  
  
  
} 
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO FOLLOW ASSIGNED PATH 

void followPath() //function to follow path set which is go to i1-B1-B2-B3-B4-i4-A4-A3-A2-A1-i1-B1-B2-B3-B4-B5
{
  
//---------------------------------------------------------------------------------------------------------------------------
//SPECIAL STATEMENTS FOR COG CONTROL
  //cog3 = cogstart((void*)goalLocations, NULL, stack3, sizeof(stack3));
  
  
//---------------------------------------------------------------------------------------------------------------------------  
  counter = 0;
  while(intersectionCount() == 0) //letting the bot reach i1
  {
    lineFollow(); //line follow
  }     
  stop(); //stopping the bot
  for (int i =0; i<=250;i++) //for loop
  {
    backward(); //going back
  }
  for(int i =0;i<=100;i++)
  {
    stop();  
  }    
  autoRotateLeft(); //rotate left
  while(intersection() == 0)
  {
   lineFollow(); 
  }    
  while(intersection() == 1)
  {
   lineFollow(); 
  }    
  while(intersectionCount() == 1) //letting the bot reach B1
  {
    lineFollow(); //line follow 
  }
  stop(); //stopping the bot
  for (int i =0; i<=350;i++) //for loop
  {
    backward(); //going back
  }
  autoRotateRight(); //rotate right
  for(int i =0; i<100;i++)
  {
    stop(); 
  }
  cog3 = cogstart((void*)goalLocationsTest, NULL, stack3, sizeof(stack3));
  while(intersection() == 0)
  {
   lineFollow(); 
  }       
  while(intersection() == 1)
  {
    lineFollow(); 
  }    
  while(intersectionCount() != 5) //letting the bot reach B4
  {
    lineFollow(); //line follow
  }
  stop(); //stopping the bot
  cogstop(cog3);
  for (int i =0; i<=250;i++) //for loop
  {
    backward(); //going back
  }
  for(int i =0;i<=100;i++)
  {
    stop();  
  }  
  autoRotateRight(); //rotate right
  for(int i =0; i<100;i++)
  {
    stop();
  }
  cog3 = cogstart((void*)goalLocationsTest, NULL, stack3, sizeof(stack3));
  while(intersection() == 0)
  {
   lineFollow(); 
  }       
  while(intersection() == 1)
  {
    lineFollow(); 
  } 
  while(intersectionCount() != 7) //letting the bot reach B4
  {
    lineFollow(); 
  }    
  stop(); //stopping the bot
  cogstop(cog3);
  for (int i =0; i<=250;i++) //for loop
  {
    backward(); //going back
  }
  for(int i =0;i<=100;i++)
  {
    stop();  
  }  
  autoRotateRight(); //rotate right
  cog3 = cogstart((void*)goalLocationsTest, NULL, stack3, sizeof(stack3));
  for(int i =0; i<100;i++)
  {
    stop(); 
  }
  while(intersection() == 0)
  {
   lineFollow(); 
  }       
  while(intersection() == 1)
  {
    lineFollow(); 
  }
  while(intersectionCount() != 10)
  {
    lineFollow();
  }
  cogstop(cog3);
  stop(); //stopping the bot
  for (int i =0; i<=250;i++) //for loop
  {
    backward(); //going back
  }
  for(int i =0;i<=100;i++)
  {
    stop();  
  }  
  autoRotateRight(); //rotate right
  for(int i =0; i<100;i++)
  {
    stop(); 
  }
  cog3 = cogstart((void*)goalLocationsTest, NULL, stack3, sizeof(stack3));
  while(intersection() == 0)
  {
   lineFollow(); 
  }       
  while(intersection() == 1)
  {
    lineFollow(); 
  }
  while(intersectionCount() != 12)
  {
    lineFollow();
  }   
  stop();
  cogstop(cog3);
  for (int i =0; i<=350;i++) //for loop
  {
    backward(); //going back
  }
  for(int i =0;i<=100;i++)
  {
    stop();  
  }  
  autoRotateRight(); //rotate right
  for(int i =0; i<100;i++)
  {
    stop(); 
  }
  cog3 = cogstart((void*)goalLocationsTest, NULL, stack3, sizeof(stack3));
  while(intersection() == 0)
  {
   lineFollow(); 
  }       
  while(intersection() == 1)
  {
    lineFollow(); 
  }
  while(intersectionCount() != 16)
  {
    lineFollow();
  }   
  stop();
  
//---------------------------------------------------------------------------------------------------------------------------
  
  //SPECIAL STATEMENTS FOR COG CONTROL
  cogstop(cog1); //ending the cog1
  cogstop(cog3); //ending the cog3
  
//---------------------------------------------------------------------------------------------------------------------------
  
}

//---------------------------------------------------------------------------------------------------------------------------  
//FUNCTION FOR GOAL LOCATIONS
/*
void goalLocations() //function to get how many goal locations it has seen so far
{
  static unsigned int truthObj = 0; //fake object to avoid multiple counts
  while(goalsDetected != 2) //while loop for excetuting till it has seen all goal locations
  {
    if((intersection() == 1) && (distance(sideTrigPin, sideEchoPin) <= 10) && (truthObj == 0)) //checking if distance is less and 15c and to ensure no recounts happens
    {
       
       goalsDetected++; //incrementing the goal locations
       ledOn(object); //turning on led
       truthObj = 1;
    }          
    else if((intersection() == 1) && truthObj == 1) //else statement
    {
      ledOn(object);
      truthObj = 1; //setting the fake object to 0
    }
    else if((distance(sideTrigPin, sideEchoPin) >= 11))
    {
      ledOff(object);
      truthObj = 0; 
    }    
    print("%d\n", goalsDetected);       
  }
//---------------------------------------------------------------------------------------------------------------------------
  //SPECIAL STATEMENTS FOR COG CONTROL
  
  cogstop(cog1); //ending the cog2
  stop(); //stopping the bot (for safety)
  cogstop(cog3); //ending the cog3

  
//---------------------------------------------------------------------------------------------------------------------------
      
} 
  */

 void goalLocationsTest() //forward declaration of function to get how many goal locations it has seen so far
 {
   while(1)
   {
   if (distance(sideTrigPin, sideEchoPin) <= 7)
   {
     ledOn(object); 
   }     
   else if((distance(sideTrigPin, sideEchoPin) > 10))
   {
     ledOff(object); 
   }   
  }     
 }   


//---------------------------------------------------------------------------------------------------------------------------
