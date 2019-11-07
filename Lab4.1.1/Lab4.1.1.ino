/*Lab4.1.1
 * Author: Jiacan Xu
 * 2 late day submission
 */

void setup() {
  Serial.begin(115200);
  pinMode(4,OUTPUT);
  pinMode(2,INPUT);

}

void loop() {
  digitalWrite(4,LOW);
  Serial.println("Switch Depressed");
  delay(10);
  while (digitalRead(2) == 1); // Wait while the switch is not pressed
  Serial.println("Switch Pressed.");
  digitalWrite(4,HIGH);
  while (digitalRead(2) == 0); // Wait while the switch is being pressed
  delay(10);
}
