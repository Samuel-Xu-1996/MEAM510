#define PIN_MOTOR_R_FWD 27
#define PIN_MOTOR_R_BWD 26
#define PIN_MOTOR_L_FWD 32
#define PIN_MOTOR_L_BWD 33

int enable1Pin = 14;
int enable2Pin = 25;

const int freq = 30000;

const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 250;

enum command {CMD_FORWARD = 'w', CMD_BACKWARD = 's', CMD_RIGHT = 'd', CMD_LEFT = 'a', CMD_STOP = 'q'};

void byteReceived(){
  char byteReceived = (char)Serial.read();

  switch (byteReceived){
    case CMD_FORWARD:
      Serial.println("Forward");
      digitalWrite(PIN_MOTOR_R_FWD, HIGH);
      digitalWrite(PIN_MOTOR_R_BWD, LOW);
      digitalWrite(PIN_MOTOR_L_FWD, HIGH);
      digitalWrite(PIN_MOTOR_L_BWD, LOW);
      ledcWrite(pwmChannel, dutyCycle);
    break;
      
    case CMD_BACKWARD:
      Serial.println("Backward");
      digitalWrite(PIN_MOTOR_R_FWD, LOW);
      digitalWrite(PIN_MOTOR_R_BWD, HIGH);
      digitalWrite(PIN_MOTOR_L_FWD, LOW);
      digitalWrite(PIN_MOTOR_L_BWD, HIGH);
      ledcWrite(pwmChannel, dutyCycle);
    break;

    case CMD_RIGHT:
      Serial.println("Right");
      digitalWrite(PIN_MOTOR_R_FWD, LOW);
      digitalWrite(PIN_MOTOR_R_BWD, HIGH);
      digitalWrite(PIN_MOTOR_L_FWD, HIGH);
      digitalWrite(PIN_MOTOR_L_BWD, LOW);
      ledcWrite(pwmChannel, dutyCycle);
    break;

    case CMD_LEFT:
      Serial.println("Left");
      digitalWrite(PIN_MOTOR_R_FWD, HIGH);
      digitalWrite(PIN_MOTOR_R_BWD, LOW);
      digitalWrite(PIN_MOTOR_L_FWD, LOW);
      digitalWrite(PIN_MOTOR_L_BWD, HIGH);
      ledcWrite(pwmChannel, dutyCycle);
    break;

    case CMD_STOP:
      Serial.println("Stop");
      digitalWrite(PIN_MOTOR_R_FWD, LOW);
      digitalWrite(PIN_MOTOR_R_BWD, LOW);
      digitalWrite(PIN_MOTOR_L_FWD, LOW);
      digitalWrite(PIN_MOTOR_L_BWD, LOW);
    break;

    default:break;
    
    
  }
}

void setup() {
  pinMode(PIN_MOTOR_R_FWD, OUTPUT);
  pinMode(PIN_MOTOR_R_BWD, OUTPUT);
  pinMode(PIN_MOTOR_L_FWD, OUTPUT);
  pinMode(PIN_MOTOR_L_BWD, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  digitalWrite(PIN_MOTOR_R_FWD, LOW);
  digitalWrite(PIN_MOTOR_R_BWD, LOW);
  digitalWrite(PIN_MOTOR_L_FWD, LOW);
  digitalWrite(PIN_MOTOR_L_BWD, LOW);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  
  Serial.begin(115200);
  

}

void loop() {
  if (Serial.available() > 0){
    byteReceived();
  }

}
