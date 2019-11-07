/* Lab4.1.2
 * Author: Jiacan Xu
 * 2 late day submission
 */
#define ANALOG_PIN A4
#define LED A5
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
}

void loop() {
    int mv;
    mv = map(analogRead(ANALOG_PIN),0,4095,50,5000);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > mv)
    {
      previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(LED, ledState);
    }
}
