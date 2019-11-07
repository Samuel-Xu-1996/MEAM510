/* Lab4.1.3
 * Author: Jiacan Xu and Geli Yang
 * 2 late day submission
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



const int ledPin=32;// the number of the LED pin

const int switchPin=25; //switch pin port mumber
#define inputPin A5     //Potentiometer Pin



int ledState = LOW;
unsigned long previousMillis = 0;        // time value stored



 
void setup() {
//set up pin mode
pinMode(inputPin, INPUT);//set pin 23 as an input
pinMode(ledPin, OUTPUT);//set pin 21 as an output
pinMode(switchPin, INPUT);
pinMode(LED_BUILTIN, OUTPUT);//let pin 2 blue as the output
Serial.begin(115200);


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
}



int lose = 201;//indicated as win
int win = 202;//as lose

int cond = 1;//int for check the wifi


void loop() {

if (cond == 1){
  int delay_time=ledControl();
  int sendTime = delay_time / 100;


      UDPsendData(sendTime);
      Serial.print("sendTime=");
      Serial.print(sendTime);
      Serial.println();
    //}
    delay(1);
    
    UDPreceiveData();//call received data

    if (receiveBuffer[0] == 202){     //I win
       cond = 2;             
       digitalWrite(2,HIGH);            //Turn on LED
       Serial.println("I win");
    } else if (receiveBuffer[0] == 201){    //I lose
       cond = 2;              
       digitalWrite(2,LOW);            //Turn off LED
       Serial.println("I lose");
    } else if ((receiveBuffer[0] != 0)&&(digitalRead(switchPin)==1)) {    
        int receivedValue = receiveBuffer[0];  //Get the first data from the received value
        //Calculate the relativepercent for determining the game
        double larger = max(receivedValue,sendTime);
        double smaller = min(receivedValue,sendTime);
        double relativepercent = (larger - smaller) / larger; 
        if (relativepercent > 0.1){     //Check if the pressed person wins or lose
         Serial.println("lalalalalala");
         int sss=0;
         while(sss<100){
         UDPsendData(win); //If I pressed the switch and I lost, win signal would be sent to the opponent
         
         sss=sss+1;//do it 100 times to make sure win is sent
         
         }
         cond = 2;              //jump out of this loop
         digitalWrite(2,LOW);            //Turn on my LED if the person loses  
         Serial.println("I lose");
        }
        else{
          int lll=0;
          while(lll<100){
           UDPsendData(lose); //If I pressed the switch and I won, lose signal would be sent to the opponent
           lll=lll+1;//do it 100 time to make sure it is sent
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

unsigned long currentMillis = millis();
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
return period;
}


void UDPsendData(int number1)
{
   

    sendBuffer[0] = number1;    
   
    // send the message
    udp.beginPacket(IPtarget, UDPtargetPort);   // target IP and target port num to send info to
    udp.printf("%s", sendBuffer);               // send the contents of sendBuffer over WiFiUDP
    udp.endPacket();
    // end message
}


void UDPreceiveData()
{
    int cb = udp.parsePacket(); // read data (check to see the number of bytes of the packet)
Serial.print("cb=");
Serial.print(cb);
Serial.println();
    if (cb)
    {
        Serial.println("Received bytes!");

        // read message and put into receiveBuffer array
       
        udp.read(receiveBuffer, packetSize);

        int receivedNumber = receiveBuffer[0];     // put the lsb and msb back into 1 int and store in variable
       
        Serial.println(""); // print an empty line

        Serial.println("jiacan's led period:");
        Serial.printf("%d\n", receiveBuffer[0]);
       

        Serial.println("End of message received");
    }
}
