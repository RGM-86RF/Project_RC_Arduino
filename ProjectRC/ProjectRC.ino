#include <BLEDevice.h>
#include "SparkFun_TB6612.h"

#define deviceName "Project RC"
#define Service_UUID "fca99450-0455-4e26-8023-7657ec9bb1eb"
#define Chara_UUID "8451f5a9-bc8b-419b-b075-3838072fda82"

#define AIN1 2
#define BIN1 5
#define AIN2 3
#define BIN2 6
#define PWMA 4
#define PWMB 7
#define STBY 9

 const int driveTime = 20;     

 const int turnTime = 8; 

 String command = "";
 int speed = 255;


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

  void onWrite(BLECharacteristic *pCharacteristic){
    std::string rxValue = pCharacteristic->getValue();

    if(rxValue.length() > 0){
      
      command = String(rxValue.c_str());
      Serial.println(command);

      if(command.toInt() >= 1 || command == "0"){
        int numCommand = command.toInt();
        speed = constrain(numCommand, 0, 255);
        Serial.println(speed);
      }

      if (command == "f") {
      
        rightMotor(speed);
        leftMotor(speed);

      }
      else if (command == "b") {
        rightMotor(-speed);
        leftMotor(-speed);
        
      }
      else if (command == "r") {
        
        rightMotor(-speed);
        leftMotor(speed);
      }
      else if (command == "l") {
        
        rightMotor(speed);
        leftMotor(-speed);
        
      }
      else if (command == "s") {
        rightMotor(0);
        leftMotor(0);
        
      }
    }
  }
};





 const int offsetA = 1;
 const int offsetB = 1;




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
   pinMode(STBY, OUTPUT);

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

  

  
}