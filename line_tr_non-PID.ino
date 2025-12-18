
//Sensor outputs 1 on black line, 0 on white floor.

#define LS 11    // Left sensor
#define CS 12    // Center sensor
#define RS 13    // Right sensor

// Motor Control Pins
#define LM1 7 
#define LM2 6  
#define RM1 5  
#define RM2 4 

#define ENA 10 
#define ENB 9  

// Motor Speeds (0 to 255)
#define BASE_SPEED 140
#define TURN_SPEED_INNER 110 // Slower speed for the inside wheel during turns
#define TURN_SPEED_OUTER 150 // Faster speed for the outside wheel

void setup() {
  pinMode(LS, INPUT);
  pinMode(CS, INPUT);
  pinMode(RS, INPUT);

  // Direction Pins
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  
  // Speed Control Pins (PWM)
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  
  // Set initial speed
  analogWrite(ENA, BASE_SPEED);
  analogWrite(ENB, BASE_SPEED);
}

void loop() {
  int left = digitalRead(LS);
  int center = digitalRead(CS);
  int right = digitalRead(RS);

  Serial.print(left); Serial.print(" ");
  Serial.print(center); Serial.print(" ");
  Serial.println(right);

 
  if ((left == 0) && (center == 1) && (right == 0)) {
    forward(BASE_SPEED, BASE_SPEED); 
  }
  
  else if ((left == 0) && (center == 1) && (right == 1)) {
    forward(TURN_SPEED_INNER, BASE_SPEED);
  }

  else if ((left == 1) && (center == 1) && (right == 0)) {
    forward(BASE_SPEED, TURN_SPEED_INNER);
  }

  else if ((left == 0) && (center == 0) && (right == 1)) {
    rightTurn(BASE_SPEED, BASE_SPEED); 
  }

  else if ((left == 1) && (center == 0) && (right == 0)) {
    leftTurn(BASE_SPEED, BASE_SPEED);
  }

  else if ((left == 1) && (center == 1) && (right == 1)) {
    stopBot();
    delay(100);
  
    reverseBot(BASE_SPEED);
    delay(120);   
    stopBot();
  }
  else {
    
    reverseBot(BASE_SPEED);
    delay(120);
    stopBot();
  }
  delay(20);
  stopBot();
}

void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  analogWrite(ENA, leftSpeed);
  
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  analogWrite(ENB, rightSpeed);
}

void rightTurn(int leftSpeed, int rightSpeed) {
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  analogWrite(ENA, leftSpeed);

  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  analogWrite(ENB, rightSpeed);
}

void leftTurn(int leftSpeed, int rightSpeed) {
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  analogWrite(ENA, leftSpeed);

  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  analogWrite(ENB, rightSpeed);
}

void reverseBot(int speed) {
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  analogWrite(ENB, speed);
}

void stopBot() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
 
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
}