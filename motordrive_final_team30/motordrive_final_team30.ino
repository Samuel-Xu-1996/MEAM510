/* Lab4.2
 * Author: Jiacan Xu & Geli Yang
 */

//WIFI setup
#include <WiFi.h>
#include <WiFiUDP.h>

// WiFi name
const char* ssid = "meam510team30";       // this is the network name
const char* pass = "123456789";         // this is the password

// IP Addresses
IPAddress myIP(192,168,1,183);      // initialize the local IP address
IPAddress friendIP(192,168,1,180);    // initialize the other device's IP address
IPAddress IPtarget=friendIP;

// variables for UDP
WiFiUDP udp;
unsigned int UDPtargetPort = 2200;  // UDP port number on the target ESP
unsigned int UDPlocalPort = 2100;   // UDP port number for local ESP
const int packetSize = 20;          // define packetSize (length of message)
byte sendBuffer[packetSize];        // create the sendBuffer array
byte receiveBuffer[packetSize];     // create the receiveBuffer array

const int freq = 30000;//pwm frequency

const int pwmChannel_1 = 0;//initialize pwm channel1 for wheel left

const int pwmChannel_2 = 1;//initialize pwm channe21 for wheel right
const int resolution = 8;//8 bit resolution pwm mode


//pins to drive motors
int MotorLeft[2] = {32,33};  //pin 32 and 33 are associated with moter left
int MotorRight[2] = {26,27};//pin 26 and 27 are associated with moter left
int enablePin1 = 25;//pwm signal left
int enablePin2 = 14;//pwm signal right
void MotorInit()
{
  int i;
  for(i=0 ; i<2; i++)
  {
  pinMode(MotorLeft[i],OUTPUT);
  pinMode(MotorRight[i],OUTPUT);
  }
}

void setup() {
  Serial.begin(115200);


  pinMode(enablePin1, OUTPUT);//set pin1 as output
  pinMode(enablePin2, OUTPUT);//enable pin 2 as output
  ledcSetup(pwmChannel_1, freq, resolution);//ledc set up for pwm channel1
  ledcAttachPin(enablePin1, pwmChannel_1);//ledc attach pwm channel1 to enable pin1


  ledcSetup(pwmChannel_2, freq, resolution);//same for pwm channel 2
  ledcAttachPin(enablePin2, pwmChannel_2);

  
  Serial.print("Connecting to: ");
  Serial.println(ssid);  
    
  WiFi.begin(ssid);           // connect to network (ssid)
  // WiFi.begin(ssid, pass);  // connect to network with password

  IPAddress gateway(192,168,1,1);         // init gateway IP
  IPAddress subnet(255,255,255,0);        // init subnet mask
  WiFi.config(myIP, gateway, subnet);     // set IP address of ESP

  udp.begin(UDPlocalPort);    // configure a port for UDP comms

    
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
   }
  Serial.println("WiFi connected!");  //print if wifi is connected
  
  MotorInit();
  Serial.print("*Motor initialization complete.");
  
}

int Stop = 100;//100 means stop
int Forward = 101;//101 means go forward
int Backward = 102;//102 means backward.
int Left = 103;//make left turn
int Right = 104;//make right turn

int dutyCycle=255; // maximum speed when going forward and going backward
void loop() {
  UDPreceiveData();// call received data

  if (receiveBuffer[0] == Stop){
    Robot_Stop();    //call stop 
  }
  else if (receiveBuffer[0] == Forward){
    Robot_Forward();//call forward function
  }
  else if (receiveBuffer[0] == Backward){   
    Robot_Backward();//call backward function
  }
  else if (receiveBuffer[0] == Left){
    Robot_Left();//call left turn function
    delay(7);//delay 7 ms 
    Robot_Stop();
  }
  else if (receiveBuffer[0] == Right){
    Robot_Right();//call right turn function
    delay(7);//delay 7 ms
    Robot_Stop();
  }
  

}

void UDPreceiveData()
{
    int cb = udp.parsePacket(); // read data (check to see the number of bytes of the packet)
    Serial.print("cb=");
    Serial.println(cb);
    if (cb)
    {
        Serial.println("Received bytes!");

        // read message and put into receiveBuffer array
       
        udp.read(receiveBuffer, packetSize);

        int receivedNumber = receiveBuffer[0];     // put the lsb and msb back into 1 int and store in variable
       
        Serial.println(""); // print an empty line

        Serial.printf("%d\n", receiveBuffer[0]);
       

        Serial.println("End of message received");
    }
}

void Robot_Forward()
{
  ledcWrite(pwmChannel_1, 235);//adjusted pwm and dutycycle to make staright forward and backward path
  ledcWrite(pwmChannel_2, dutyCycle);
   digitalWrite(MotorLeft[0],0);
   digitalWrite(MotorLeft[1],1);
   digitalWrite(MotorRight[0],1);
   digitalWrite(MotorRight[1],0);   
}
void Robot_Backward()
{
  ledcWrite(pwmChannel_1, 235);
  ledcWrite(pwmChannel_2, dutyCycle);//adjusted pwm and dutycycle to make staright forward and backward path
   digitalWrite(MotorLeft[0],1);
   digitalWrite(MotorLeft[1],0);
   digitalWrite(MotorRight[0],0);
   digitalWrite(MotorRight[1],1);  
}
void Robot_Left()
{
  ledcWrite(pwmChannel_1, 180);//adjusted pwm to turn make smaller amount of angle change
  ledcWrite(pwmChannel_2, 180);
  digitalWrite(MotorLeft[0],1);
  digitalWrite(MotorLeft[1],0);
  digitalWrite(MotorRight[0],1);
  digitalWrite(MotorRight[1],0);    
}
void Robot_Right()
{
  ledcWrite(pwmChannel_1, 180);
  ledcWrite(pwmChannel_2, 180);//adjusted pwm to turn make smaller amount of angle change
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
