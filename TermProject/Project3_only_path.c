// ALL INCLUDE STATEMENTS

#include "servo.h"
#include "fdserial.h"
#include "servodiffdrive.h"
#include "simpletools.h"

//---------------------------------------------------------------------------------------------------------------------------

//ALL COGS

int *cog1; //cog 1 reference
int *cog2; //cog 2 reference
int *cog3; //cog 3 reference

//---------------------------------------------------------------------------------------------------------------------------

//ALL STACKS



unsigned int stack1[40+100]; //stack1 value
unsigned int stack2[40+100]; //stack2 value
unsigned int stack3[40+100]; //stack3 value


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
#define pinIntersectionIndicator 9 //pin for led for intersection indication
#define sideTrigPin 6 //trigger Pin of side(left) ultrasonic sensor
#define sideEchoPin 7 //trigger Pin of side(left) ultrasonic sensor
#define ledPin 10 //led pin for obstacle detectiions indication
#define objectFriendly 11 //object detection led for a friendly
#define objectEnemy 4 //object detection led for an enemy
#define knocker 17 // standard servo for knocking


//---------------------------------------------------------------------------------------------------------------------------

//GLOBAL VARIABLES
fdserial *tags; //serial object for communication with rasberry pie
volatile char com; //variable to store values of serial communication
volatile unsigned int counter = 0; // global counter variable
volatile unsigned int middleCounter = 0; // global counter variable
unsigned int locationOfJam; //global int storing location of traffic jam
unsigned int goalsDetected = 0; //global int for storing how many goal locations it has seen
unsigned int nowStop = 0; //global int for signifing stopping
unsigned int nowKnock = 0; //global unsigned int signifing knock

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
void stop(const unsigned int time); //forward declaration of  stationary function
void lineFollow(); //forward declaration of line following function
int intersection(); //forward declaration of detecting an intersection
int intersectionCount(); //forward declaration of intersection counter
int frontObj(); //forward declaration of function to see if there is a front object
void rotateLeft(); // forward declaration of function to rotate the bot left
void rotateRight(); //forward declaration of function to rotate the bot right
void detectJamAndReturn(); //forward declaration of function to find where Jam is and come back to start 
void autoRotateLeft(); //forward declaration of function to rotate left till it finds black line
void autoRotateRight(); //forward declaration of function to rotate right till it finds black line
void followPath(); //forward declaration of function to follow path set which is got to B1-B2-B3-B4-i4-A4-A3-A2-A1-i1-B1-B2-B3-B4-B5
void exitIntersection(const unsigned int translation); //forward declaration of function to exit intersection
int middleIntersectionCount(); //forward declaration of middle lane intersection counter
void initSer(); //forward declaration of function to intialize serial communication
void detectArUcoTags(); //forward declaration of function to detect arUco tags
void knock(); //foward declaration of function to knock enemy


//---------------------------------------------------------------------------------------------------------------------------

// MAIN LOOP


int main()
{  
  servoInit(leftServoPin, rightServoPin); //initalizing servo
  initSer(); //instalizing the serial communication
  cog1 = cogstart((void*)detectJamAndReturn, NULL, stack1, sizeof(stack1)); //starting cog1
  cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2));  //starting cog2 
  cog3 = cogstart((void*)knock, NULL, stack3, sizeof(stack3)); //starting cog 3      
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
//FUNCTION TO TAKE IR READINGS

int IR(const unsigned int pin) //function to get IR input
{
  volatile int i = input(pin); //volatile int storing input of pin
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
  //print("%d cm\n", cmDist);
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
  drive_speeds(-30,-30); //full forward
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DRIVE BACKWARD
void backward() //drive backward
{
  drive_speeds(30,30); //full backward
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TILT LEFT
void left() //turn left
{
  drive_speeds(-30, 0); //keeping left wheel stationary
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO TILT RIGHT
void right() //turn right
{
  drive_speeds(0, -30); //keeping right wheel stationary
} 
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR STOPPING
void stop(const unsigned int time) //bot stationary
{
  for (int i = 0; i <= time; i++)
  {
    drive_stop(); //stopping the servos
  }   
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO ROTATE RIGHT
void rotateRight() //bot rotate right
{
  drive_speeds(30, -30); //rotating both wheels same speed in opposite direction to make it rotate right 
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO ROATE LEFT
void rotateLeft() //bot rotate left
{
  drive_speeds(-30, 30); //rotating both wheels same speed in opposite direction to make it rotate left
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR AUTO ROTATION TO LEFT
void autoRotateLeft() //function to rotate left till it finds black line
{   
  while(IR(pinIrMiddleRight) == 1) //while loop to get out of black line
  {
    rotateLeft(); //rotate left
  }
  stop(300); //stop motors
  while(IR(pinIrMiddleRight) == 0) // while loop to get back into black line
  {
    rotateLeft(); //rotate left
  }
  stop(300); //stop motors  
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR AUTO ROTATION RIGHT
void autoRotateRight() //function to rotate right till it finds black line
{  
  while(IR(pinIrMiddleLeft) == 1) //while loop to get out of black line
  {
    rotateRight(); //rotate right
  }
  stop(300); //stop motors  
  while(IR(pinIrMiddleLeft) == 0) // while loop to get back into black line
  {
    rotateRight(); //rotate right
  }
  stop(300); //stop motors    
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
    stop(1); //stop
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
  return counter; //returning the count
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION FOR COUNTING INTERSECTION ONLY FOR THE MIDDLE LANE
int middleIntersectionCount() //function to count the intersection
{
  
  static unsigned int middleTruth = 0; //fake object to avoid multiple counts
  
  if (intersection() == 1 && middleTruth == 0) //checking if there is an intersection and avoid recounts
  {
    middleCounter++; //incrementing the counter
  }    
  middleTruth = intersection(); //setting the fake object to see if it still at an intersection
  return middleCounter; //returning the count
}


//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DETECT WHERE THE JAM IS AT RETURN ITS LOCATION
int frontObj() //function to see if there is an object in front
{
  unsigned int trafficAt = intersectionCount(); 
  if (distance(frontTrigPin, frontEchoPin) <= 2) //checking if distance is less than or equal to 5
  {
    //print("Traffic jam at %d\n", trafficAt +1); //printing for testing
    ledOn(ledPin); //truning on led for front object detection
    locationOfJam = trafficAt + 1; //setting global variable to store where Jam is located
    return trafficAt + 1; //return the location of the jam
  }    
  else //else statement
  {
    return 0; //returns 0 if false
  }    
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO EXIT INTERSECTION
void exitIntersection(const unsigned int translation) //function to exit intersection
{      
  while(intersection() == 1) //while loop to exit intersection
  {
    lineFollow(); //line follow
  } 
  for(int i = 0; i <= translation; i++) //for loop to give a very minor translation
  {
    lineFollow(); // line loop
  }    
  stop(60); //stopping the bot for saftey
}  

//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO START FRO HOME AND COME BACK TO I0 AFTER DETECTING WHERE THE JAM IS AT
void detectJamAndReturn() //function to find where Jam is and come back to start 
{
  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  while((frontObj() != 3) && (frontObj() != 4) && (frontObj() != 6) ) //while loop to ensure line follow keep executing till condition not satisfied
  {
    lineFollow(); //line follow
    //detectArUcoTags(); //detect tags
  } 
  //cogstop(cog2); //stopping cog2 
  counter = 0; //resetting the counter
  stop(50); //stopping the bot for smooth motion 
  for (int i = 0; i <= 600; i++) //for loop for going back
  {
    backward(); //bakward
  }    
  autoRotateLeft(); //180 degree rotation
  ledOff(ledPin); //swtiching off led for front object detection

  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //stopping cog2
  switch (locationOfJam) //switch case for the where the jam is at
  {
    case 3 : //case 3
      while(intersectionCount() != 1) //till count = 2 it will keep following the line
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }
      stop(50); //stopping the bot for smooth motion 
      break; //break statement
    case 4 : //case 4
      while(intersectionCount() != 2) //till count = 3 it will keep following the line
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }
      stop(50);  //stopping the bot for smooth motion 
      break; //break statement
    case 6 : //case 6
      while(intersectionCount() != 4) //till count = 5 it will keep following the line
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }
      stop(50); //stopping the bot for smooth motion 
      break;  //break statement     
  }
  stop(1);  //stopping the bot for smooth motion 
  //cogstop(cog2); //stoping cog2 
  exitIntersection(400); //exit intersection
  autoRotateRight(); //90 degree rotation   
  stop(1); //stop
  counter = 0; //resetting the counter
  //cog2 = cogstart((void*)followPath, NULL, stack2, sizeof(stack2)); //starting cog2
  //cogstop(*cog1);
  followPath(); //starting next function follow Path on the same cog
  
  
  
} 
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO FOLLOW ASSIGNED PATH 

void followPath() //function to follow path set which is go to B1-B2-B3-B4-i4-A4-A3-A2-A1-i1-B1-B2-B3-B4-B5
{

  counter = 1; //resetting the counter to 1 for safety
  while(intersectionCount() != 2) //letting the bot reach B1
  {
    lineFollow(); //line follow
  }   
  stop(1); //stopping the bot  
  exitIntersection(200); //exit intersection //NEW
  autoRotateRight(); //rotate right 
  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  while(intersectionCount() != 5) //letting the bot reach B4
  {
    lineFollow(); //line follow 
    //detectArUcoTags(); //detect tags
  }
  //cogstop(cog2);  
  stop(1); //stopping the bot
  exitIntersection(600); //exit intersection //NEW
  autoRotateRight(); //rotate right 
  while(intersectionCount() != 6) //letting the bot reach i4
  {
    lineFollow(); //line follow
  }
  stop(1); //stopping the bot
  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  switch (locationOfJam) //switch case for the where the jam is at
  {
    case 3:
      exitIntersection(400); //exit intersection //NEW
      autoRotateRight(); //rotate 90 right
      //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
      while ((middleIntersectionCount() <= 1) && (distance(frontTrigPin, frontEchoPin) >= 2)) //while loop to keep following line till front abject is detected
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }
      stop(100); //stop
      ledOn(ledPin); //turning on led
      stop(100);
      for (int i = 0; i <= 800; i++) //for loop for going back
      {
        backward(); //bakward
      } 
      //cogstop(cog2); //stopping cog 2
      autoRotateRight(); //rotate 180 right 
      stop(100);
      ledOff(ledPin); // turning off led
      //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
      while (middleIntersectionCount() != 4) //while loop for the bot to reach i5
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }            
      stop(1); //stop
      //cogstop(cog2); //cog stop
      exitIntersection(400); //exit intersection //NEW
      autoRotateRight(); //rotate 90 right 
      stop(2000); //a big stop statement to for a smooth trasistion      
      autoRotateRight(); //rotate 90 right 
      stop(1); //stop
      //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
      while (middleIntersectionCount() != 5) //let the bot reach i4
      {
        lineFollow(); //line following 
        //detectArUcoTags(); //detect tags
      }    
      //cogstop(cog2); //cog stop 2
      exitIntersection(400); //exit intersection //NEW    
      autoRotateLeft(); //rotate 90 left
      break; //break statement
    case 4:
      exitIntersection(400); //exit intersection //NEW
      autoRotateRight(); //rotate 90 right
      //stop(1000); //stopping for a very less time to avoid front detection early
      //cog3 = cogstart((void*)detectArUcoTags2, NULL, stack3, sizeof(stack3)); //starting cog2 
      while ((middleIntersectionCount() == 0) && (distance(frontTrigPin, frontEchoPin) >= 2)) //while loop to keep following line till front abject is detected
      {
        lineFollow(); //line follow
        //detectArUcoTags(); //detect tags
      }
      //cogstop(cog3); //cog stop 2
      stop(100);
      ledOn(ledPin); //turning on led
      stop(100);
      for (int i = 0; i <= 800; i++) //for loop for going back
      {
        backward(); //bakward
      } 
      autoRotateRight(); //rotate 180 right 
      stop(100);
      ledOff(ledPin); // turning off led
      //cog3 = cogstart((void*)detectArUcoTags2, NULL, stack3, sizeof(stack3)); //starting cog2 
      while (middleIntersectionCount() != 2) //while loop for the bot to reach i5
      {
        lineFollow(); //line follow 
        //detectArUcoTags(); //detect tags
      }          
      //cogstop(cog3); //cog stop 2  
      stop(1); //stop
      exitIntersection(400); //exit intersection //NEW
      autoRotateRight(); //rotate 90 right 
      stop(2000); //a big stop statement to for a smooth trasistion     
      autoRotateRight(); //rotate 90 right 
      stop(1); //stop
      //cog3 = cogstart((void*)detectArUcoTags2, NULL, stack2, sizeof(stack2)); //starting cog2 
      while (middleIntersectionCount() != 3) //let the bot reach i4
      {
        lineFollow(); //line following 
        //detectArUcoTags(); //detect tags
      }        
      //cogstop(cog3); //cog stp 2
      exitIntersection(400); //exit intersection //NEW
      autoRotateLeft(); //rotate 90 left
      break; //break statement
    case 6:
      exitIntersection(400); //exit the current intersection
      break; //break statement
  }    
  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  while(intersectionCount() != 7) //letting the bot reach A4
  {
    lineFollow(); //line follow
  }
  stop(1); //stopping the bot  
  exitIntersection(400); //exit intersection //NEW 
  autoRotateRight(); //rotate right  
  //cog4 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  while(intersectionCount() != 10) //letting the bot reach A1
  {
    lineFollow(); //line follow
    //detectArUcoTags(); //detect tags
  }
  stop(1); //stopping the bot 
  //cogstop(cog4); //cog stop 2
  exitIntersection(600); //exit intersection //NEW  
  autoRotateRight(); //rotate right  
  while(intersectionCount() != 12) //letting the bot reach B1
  {
    lineFollow(); //line follow
  }
  stop(1); //stopping the bot 
  exitIntersection(400); //exit intersection //NEW  
  autoRotateRight(); //rotate right
  while(intersectionCount() != 15) //letting bot reach B4
  {
    lineFollow(); //line follow
  }     
  //cog2 = cogstart((void*)detectArUcoTags, NULL, stack2, sizeof(stack2)); //starting cog2
  while(intersectionCount() != 16) //letting the bot reach B5
  {
    lineFollow(); //line follow
    //detectArUcoTags(); //detect tags
  }
  exitIntersection(800);
  stop(1);  
      
  
//---------------------------------------------------------------------------------------------------------------------------
  
  //SPECIAL STATEMENTS FOR COG CONTROL
  //cogstop(*cog2); //ending the cog2
  
//---------------------------------------------------------------------------------------------------------------------------
  
}
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO INTIALIZE SERIAL COMMUNICATION
void initSer() //function to intialize serial communication
{
  tags = fdserial_open(31, 30, 0, 115200); 
}  
//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO DETECT TAGS

void detectArUcoTags() //function to detect arUco Tags
{
  initSer(); //instalizing the serial communication
  while(1){
  com = fdserial_rxChar(tags); //reading the communication from rasberry pie which encodes as UTF-8
  if((distance(sideTrigPin, sideEchoPin) >= 3 && distance(sideTrigPin, sideEchoPin) <= 12)) //if ultrasonic detects an object
  {
    if (com != -1) //only if com is transmitting data it will exceute
    {
      if (com == '2') //if char is 2 it means no tags Ids were detected (false positive from Ultrasonic sensor)
      {
        ledOff(objectFriendly); //led off 
        ledOff(objectEnemy); //led off
        nowKnock = 0; //knocking flag set to false
      }
      else if (com == '0') //if rasberry pie transmites 0 which means friendly
      {
        ledOn(objectFriendly); //led on for friendly object
        ledOff(objectEnemy); //led off for enemy
        nowKnock = 0; //knocking flag set to false
      }        
      else if (com == '1') //if rasberry pie transmites 1 which means enemy
      {
        ledOff(objectFriendly); //led off for friendly object
        ledOn(objectEnemy); //led on object enemy
        nowKnock = 1; //Knocking flag set to true 
      }
      else
      {
        ledOff(objectFriendly); //led off for friendly object
        ledOn(objectEnemy); //led on object enemy
        nowKnock = 0; //Knocking flag set to true 
      }                
    }       
  } 
  else
  {
    ledOff(objectFriendly); //led off 
    ledOff(objectEnemy); //led off
    nowKnock = 0; //Knocking flag set to false
  }
}         
}

//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION TO KNOCK ENEMIES
void knock() //function to knock enemy
{
  while(1) //infinite loop
  {
    if(nowKnock == 1) //signifing knocking as 1
    {
      servo_angle(knocker, 700); //knocking to 90 deg  
    }
    else if (nowKnock == 0)
    {
      servo_angle(knocker, 1800); //knocking to 0 deg  
    }             
  }     
}
//---------------------------------------------------------------------------------------------------------------------------  