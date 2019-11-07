// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int motor2Pin1 = 32;
int motor2Pin2 = 33;

int enable1Pin = 14;
int enable2Pin = 25;

// Setting PWM properties
const int freq = 30000;

const int pwmChannel_1 = 0;
const int pwmChannel_2 = 1;
const int resolution = 8;
int dutyCycle = 200;

void setup(){
  // set the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // configure LED PWM functionalities
  //ledcSetup(pwmChannel_1, freq, resolution);
  ledcSetup(pwmChannel_2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  //ledcAttachPin(enable1Pin, pwmChannel_1);
  ledcAttachPin(enable2Pin, pwmChannel_2);
  

  Serial.begin(115200);

  //testing
  Serial.print("Testing DC Motor...");
}

void loop(){
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  //digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor2Pin1, LOW);
  //digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin2, HIGH);
  delay(2000);

  //stop the DC motor
  Serial.println("Motor Stopped");
  //digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor2Pin1, LOW);
  //digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

  //Move DC motor backwards at mximum speed
  Serial.println("Moving Backwards");
  //digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  //digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(2000);

  //Move DC motor forward with increasing speed
  //digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  //digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
  while (dutyCycle <= 255){
    int dutyCycle_1 = dutyCycle;
    int dutyCycle_2 = dutyCycle;
    //ledcWrite(pwmChannel_1, dutyCycle_1);
    ledcWrite(pwmChannel_2, dutyCycle_2);
    Serial.println("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
}
