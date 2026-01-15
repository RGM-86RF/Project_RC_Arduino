#include <BLEDevice.h>
#include "SparkFun_TB6612.h"

#define deviceName "Project RC"
#define Service_UUID "fca99450-0455-4e26-8023-7657ec9bb1eb"
#define Chara_UUID "8451f5a9-bc8b-419b-b075-3838072fda82"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer){
    digitalWrite(13,HIGH);
    Serial.println("Client Connected");
  }

  void onDisconnect(BLEServer *pServer){
    digitalWrite(13,LOW);
    Serial.println("Client Disconnected");
    BLEDevice::startAdvertising();
  }
};

class myCharaCallbacks : public BLECharacteristicCallbacks{
  void onRead(BLECharacteristic *pCharacteristic){
    uint32_t currentMillis = millis() / 1000;
    pCharacteristic->setValue(currentMillis);
  }
};



#define AIN1 2
#define BIN1 5
#define AIN2 3
#define BIN2 6
#define PWMA 4
#define PWMB 7
#define STBY 9


 //const int offsetA = 1;
 ///const int offsetB = 1;


 //Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
 //Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);


 const int driveTime = 20;      //this is the number of milliseconds that it takes the robot to drive 1 inch
                                //it is set so that if you tell the robot to drive forward 25 units, the robot drives about 25 inches

 const int turnTime = 8;        //this is the number of milliseconds that it takes to turn the robot 1 degree
                                //it is set so that if you tell the robot to turn right 90 units, the robot turns about 90 degrees

                                //Note: these numbers will vary a little bit based on how you mount your motors, the friction of the
                                //surface that your driving on, and fluctuations in the power to the motors.
                                //You can change the driveTime and turnTime to make them more accurate

 String botDirection;           //the direction that the robot will drive in (this change which direction the two motors spin in)
 String distance;               //the distance to travel in each direction

/********************************************************************************/
 void motorSetup()
 {

   //set the motor control pins as outputs
   pinMode(AIN1, OUTPUT);
   pinMode(AIN2, OUTPUT);
   pinMode(PWMA, OUTPUT);

   pinMode(BIN1, OUTPUT);
   pinMode(BIN2, OUTPUT);
   pinMode(PWMB, OUTPUT);

   digitalWrite(STBY, HIGH);

   //prompt the user to enter a command
   Serial.println("Enter a direction followed by a distance.");
   Serial.println("f = forward, b = backward, r = turn right, l = turn left");
   Serial.println("Example command: f 50");
 }










void setup()
{
 Serial.begin(115200);
 while (!Serial && (millis() < 5000))
 pinMode(13,OUTPUT);

 BLEDevice::init(deviceName);

 BLEServer *pServer = BLEDevice::createServer();
 pServer->setCallbacks(new MyServerCallbacks());

 BLEService *pService = pServer->createService(Service_UUID);

 BLECharacteristic *pCharacteristic = pService->createCharacteristic(Chara_UUID,
 BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

 pCharacteristic->setCallbacks(new myCharaCallbacks());

 pService->start();

 BLEDevice::startAdvertising();


 motorSetup();
 
 
}


void loop()
{                                                   
    if (Serial.available() > 0)                         //if the user has sent a command to the RedBoard
    {
      botDirection = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space
      distance = Serial.readStringUntil(' ');           //read the characters in the command until you reach the second space

      //print the command that was just received in the serial monitor
      Serial.print(botDirection);
      Serial.print(" ");
      Serial.println(distance.toInt());

      if (botDirection == "f")                         //if the entered direction is forward
      {
        rightMotor(200);                                //drive the right wheel forward
        leftMotor(200);                                 //drive the left wheel forward
        delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
      }
      else if (botDirection == "b")                    //if the entered direction is backward
      {
        rightMotor(-200);                               //drive the right wheel forward
        leftMotor(-200);                                //drive the left wheel forward
        delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
      }
      else if (botDirection == "r")                     //if the entered direction is right
      {
        rightMotor(-200);                               //drive the right wheel forward
        leftMotor(255);                                 //drive the left wheel forward
        delay(turnTime * distance.toInt());             //drive the motors long enough turn the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
      }
      else if (botDirection == "l")                   //if the entered direction is left
      {
        rightMotor(255);                                //drive the right wheel forward
        leftMotor(-200);                                //drive the left wheel forward
        delay(turnTime * distance.toInt());             //drive the motors long enough turn the entered distance
        rightMotor(0);                                  //turn the right motor off
        leftMotor(0);                                   //turn the left motor off
      }
    }
 
}
/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
 
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  motorSpeed *= -1;
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}