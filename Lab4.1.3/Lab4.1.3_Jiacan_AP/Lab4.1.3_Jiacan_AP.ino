/* Lab4.1.3
 * Author: Jiacan Xu
 * 2 late day submission
 */

//WIFI setup
#include <WiFi.h>
#include <WiFiUDP.h>

// WiFi name
const char* ssid = "meam510team30";       // this is the network name
const char* pass = "123456789";         // this is the password

// IP Addresses
IPAddress myIP(192,168,1,180);      // initialize the local IP address
IPAddress friendIP(192,168,1,183);    // initialize the other device's IP address
IPAddress IPtarget=friendIP;

// variables for UDP
WiFiUDP udp;
unsigned int UDPtargetPort = 2100;  // UDP port number on the target ESP
unsigned int UDPlocalPort = 2200;   // UDP port number for local ESP
const int packetSize = 20;          // define packetSize (length of message)
byte sendBuffer[packetSize];        // create the sendBuffer array
byte receiveBuffer[packetSize];     // create the receiveBuffer array


const int ledPin = A5;// the number of the LED pin
const int switchPin = 25; //switch pin mumber
#define inputPin A4     //input Pin


// Variables will change :
int ledState = LOW;
unsigned long previousMillis = 0;        // will store last time LED was updated



 
void setup() {
//set up pin mode
pinMode(inputPin, INPUT);//set pin A4 as an input
pinMode(ledPin, OUTPUT);//set pin A5 as an output
pinMode(switchPin, INPUT);
pinMode(LED_BUILTIN, OUTPUT);
Serial.begin(115200);


  //WIFI setup
  // connect to WiFi
    Serial.print("Connecting to: "); Serial.println(ssid);  // debug statement

    WiFi.mode(WIFI_AP); // set the WiFi mode to AP mode
    WiFi.softAP(ssid); // configures the ESP in AP mode with network name
    delay(100);
    //WiFi.begin(ssid, pass);           // connect to network (ssid)
    // WiFi.begin(ssid, pass);  // connect to network with password

    IPAddress gateway(192,168,1,1);         // init gateway IP
    IPAddress subnet(255,255,255,0);        // init subnet mask
    WiFi.softAPConfig(myIP, gateway, subnet);     // set IP address of ESP

    udp.begin(UDPlocalPort);    // configure a port for UDP comms

    // hold the code here and wait until the WiFi is connected
    /*
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    */
    IPAddress myIP = WiFi.softAPIP(); // demo the usage of the softAPIP
    Serial.print("AP IP address: "); Serial.println(myIP);
    Serial.println("WiFi connected!");  
}


//result, 201 is opponent losing, 202 is opponent winning 
int lose = 201;
int win = 202;

int cond = 1;


void loop() {

if (cond == 1){
  int delay_time=ledControl();
  int sendTime = delay_time / 100;

  UDPsendData(sendTime);
  Serial.print("send time = ");
  Serial.print(sendTime);
  Serial.println();
  delay(1);
    
  UDPreceiveData();
  Serial.print("switch ==");
  Serial.print(digitalRead(switchPin)); // debug switch pin
  Serial.println();

    if (receiveBuffer[0] == 202){     //I win
       cond = 2;             
       digitalWrite(2,HIGH);            //Turn on LED
       Serial.println("I win");
    } else if (receiveBuffer[0] == 201){    //I lose
       cond = 2;              
       digitalWrite(2,LOW);            //Turn off LED
       Serial.println("I lose");
    } else if ((receiveBuffer[0] != 0) && (digitalRead(switchPin) == 1)) {    
        int receivedValue = receiveBuffer[0];  //Get the first data from the received value

        double larger = max(receivedValue,sendTime);
        double smaller = min(receivedValue,sendTime);
        double relativepercent = (larger - smaller) / larger; 
        if (relativepercent > 0.1){     //Check if the pressed person wins or lose
          int iii = 0;
          while(iii < 100){ //do a hundred times to make sure win signal is sent to opponent
         UDPsendData(win); //If I pressed the switch and I lost, win signal would be sent to the opponent
         iii = iii + 1;
          }
         cond = 2;              //jump out of this loop
         digitalWrite(2,LOW);            //Turn on my LED if the opponent loses  
         Serial.println("I lose");
        }
        else{
          int jjj = 0;
          while(jjj < 100){ //do a hundred times to make sure lose signal is sent to opponent
           UDPsendData(lose); //If I pressed the switch and I won, lose signal would be sent to the opponent
           jjj = jjj + 1;
          }
           cond = 2;         //Jump out of the loop
           digitalWrite(2,HIGH);           //Turn off LED
           Serial.println("I win");
          }
    }
}
 else if (cond == 2){       //Print game over when the result comes out
    Serial.println("Game Over");
    delay(100000);
  }
}

//FUNCTIONS

int ledControl(){

unsigned long currentMillis = millis(); // define current millis
int period=map(analogRead(inputPin),0,4095,100,10000);
if(currentMillis-previousMillis >=period/2){
  previousMillis = currentMillis;
  if(ledState==LOW){
    ledState = HIGH;
  } else{
    ledState = LOW;
  }
}

digitalWrite(ledPin, ledState);
//Serial.print("LED Blink period (in ms): ");
//Serial.println(period);
return period;
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


void UDPreceiveData()
{
    int cb = udp.parsePacket(); // read data (check to see the number of bytes of the packet)
    Serial.print("cb = ");
    Serial.print(cb);
    Serial.println();
    if (cb)
    {
        Serial.println("Received bytes!");


        udp.read(receiveBuffer, packetSize);

        int receivedNumber = receiveBuffer[0];     // put the lsb and msb back into 1 int and store in variable
       
        Serial.println(""); // print an empty line

        Serial.printf("Opponent's LED period:");
        Serial.printf("%d\n", receiveBuffer[0]);
       

        Serial.println("End of message received");
    }
}
