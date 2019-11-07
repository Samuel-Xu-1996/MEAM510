/* Lab4.2
 *  Author: Jiacan Xu
 */

//WIFI setup
#include <WiFi.h>
#include <WiFiUDP.h>

//WiFi name
const char* ssid = "meam510team30";       // this is the network name
const char* pass = "123456789";         // this is the password

// IP Addresses
IPAddress myIP(192,168,1,180);      // initialize the local IP address
IPAddress friendIP(192,168,1,183);    // initialize the other device's IP address
IPAddress IPtarget = friendIP;

// variables for UDP
WiFiUDP udp;
unsigned int UDPtargetPort = 2100;  // UDP port number on the target ESP
unsigned int UDPlocalPort = 2200;   // UDP port number for local ESP
const int packetSize = 20;          // define packetSize (length of message)
byte sendBuffer[packetSize];        // create the sendBuffer array
byte receiveBuffer[packetSize];     // create the receiveBuffer array


const int Joystick_X_Pin = A5;     // Joystick X direction Pin number
const int Joystick_Y_Pin = A4;     // Joystick Y direction Pin number
const int switchPin = A7;         // switch pin mumber

void setup() {
  Serial.begin(115200);
  pinMode(A5, INPUT);// input pin for joystick
  pinMode(A4, INPUT);//input pin for joystick
  pinMode(A7, INPUT);// input pin for joystick


  //WIFI setup
  // connect to WiFi
  Serial.print("Connecting to: "); Serial.println(ssid);  // debug statement

  WiFi.mode(WIFI_AP); // set the WiFi mode to AP mode
  WiFi.softAP(ssid); // configures the ESP in AP mode with network name
  delay(100);
  // WiFi.begin(ssid, pass);  // connect to network with password

  IPAddress gateway(192,168,1,1);         // init gateway IP
  IPAddress subnet(255,255,255,0);        // init subnet mask
  WiFi.softAPConfig(myIP, gateway, subnet);     // set IP address of ESP

  udp.begin(UDPlocalPort);    // configure a port for UDP comms
  IPAddress myIP = WiFi.softAPIP(); // demo the usage of the softAPIP
  Serial.print("AP IP address: "); Serial.println(myIP);
  Serial.println("WiFi connected!");
}

int Stop = 100;
int Forward = 101;
int Backward = 102;
int Left = 103;
int Right = 104;

void loop() {
  int Xvalue = analogRead(A5);
  int Yvalue = analogRead(A4);
  Serial.printf("X = ");  Serial.println(Xvalue);
  Serial.printf("Y = ");  Serial.println(Yvalue);
  Serial.println("-------------------------");
  if (abs(Xvalue - 1980) < 50 && abs(Yvalue - 2020) < 50){
    Serial.println("Stop");
    UDPsendData(Stop);
  }
  else if ((Xvalue + Yvalue > 4095) && (2020*Xvalue > 1980*Yvalue)){//// dividevalue into 4 region and send signal to esp32 receiver side
    Serial.println("Forward");
    UDPsendData(Forward);
  }
  else if ((Xvalue + Yvalue < 4095) && (2020*Xvalue <= 1980*Yvalue)){
    Serial.println("Backward");
    UDPsendData(Backward);
  }
  else if ((Xvalue + Yvalue <= 4095) && (2020*Xvalue > 1980*Yvalue)){
    Serial.println("Left");
    UDPsendData(Left);
  }
  else if ((Xvalue + Yvalue >= 4095) && (2020*Xvalue < 1980*Yvalue)){
    Serial.println("Right");
    UDPsendData(Right);
  }
  Serial.println("-------------------------");
  delay(100);

}

void UDPsendData(int number1)
{
    //Serial.println("Sending data...");

    // create the message to be sent
    // sendBuffer is an array where each element is 1 byte, put the things to be sent into the sendBuffer array
    // avoid/don't put 0 in sendBuffer (0 is the null terminator, and udp will not send anything after that 0)

    sendBuffer[0] = number1;    
   
    // send the message
    udp.beginPacket(IPtarget, UDPtargetPort);   
    udp.printf("%s", sendBuffer);               // send the contents of sendBuffer over WiFiUDP
    udp.endPacket();
    // end message
    
    Serial.println("Finished sending data!");
}
