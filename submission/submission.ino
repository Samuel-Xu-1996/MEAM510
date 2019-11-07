#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//pins to drive motors
int MotorLeft[2] = {32,33};  
int MotorRight[2] = {26,27};

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b-IAHfi5Dwbb-GCCLrJcoln8qV3ilFVB";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPhone";
char pass[] = "valerie1996";

 
void setup()
{
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);
MotorInit();
Serial.print("*Explore Robo Mode Computer: Controlled*\n\r");
Serial.println("Commands:\n W->Forward \n S->Backwards \n A->Left \n D->Right");
}
 
void loop()
{
  
 Blynk.run();
 
}
 
//Intialize the motor
void MotorInit()
{
  int i;
  for(i=0 ; i<2; i++)
  {
  pinMode(MotorLeft[i],OUTPUT);
  pinMode(MotorRight[i],OUTPUT);
  }
}
//Robot Driving Functions
void Robot_Forward()
{
   digitalWrite(MotorLeft[0],0);
   digitalWrite(MotorLeft[1],1);
   digitalWrite(MotorRight[0],1);
   digitalWrite(MotorRight[1],0);   
}
void Robot_Backward()
{
   digitalWrite(MotorLeft[0],1);
   digitalWrite(MotorLeft[1],0);
   digitalWrite(MotorRight[0],0);
   digitalWrite(MotorRight[1],1);  
}
void Robot_Left()
{
  digitalWrite(MotorLeft[0],1);
  digitalWrite(MotorLeft[1],0);
  digitalWrite(MotorRight[0],1);
  digitalWrite(MotorRight[1],0);    
}
void Robot_Right()
{
  digitalWrite(MotorLeft[0],0);
  digitalWrite(MotorLeft[1],1);
  digitalWrite(MotorRight[0],0);
  digitalWrite(MotorRight[1],1);    
}

void Robot_Stop()
{
  digitalWrite(MotorLeft[0],0);
  digitalWrite(MotorLeft[1],0);
  digitalWrite(MotorRight[0],0);
  digitalWrite(MotorRight[1],0);    
}



BLYNK_WRITE(V1)
{   
  int value = param.asInt(); // Get value as integer
 // Serial.println("Going Forward");
  if(value)
  {
    Robot_Forward();

  }
}

BLYNK_WRITE(V2)
{   
  int value = param.asInt(); // Get value as integer
  //Serial.println("Moving Right");
  if(value)
  {
    Robot_Right();
    delay(200);
    Robot_Stop();
  }
}


BLYNK_WRITE(V3)
{   
  int value = param.asInt(); // Get value as integer
 // Serial.println("Going back");
  if(value)
  {
    Robot_Backward();

  }
}


BLYNK_WRITE(V4)
{   
  int value = param.asInt(); // Get value as integer
  //Serial.println("Taking Left");
  if(value)
  {
    Robot_Left();
    delay(200);
    Robot_Stop();

  }
}

BLYNK_WRITE(V5)
{   
  int value = param.asInt(); // Get value as integer
 // Serial.println("Braking!!");
  if(value)
  {
    Robot_Stop();
  }
}
