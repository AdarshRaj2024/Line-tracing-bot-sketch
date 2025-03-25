// Define IR sensor pins
#define IR_LEFT_2 34
#define IR_LEFT_1 35
#define IR_MIDDLE 32
#define IR_RIGHT_1 33
#define IR_RIGHT_2 25

// Define motor control pins
#define IN1 21
#define IN2 19
#define IN3 18
#define IN4 5
#define ENA 23
#define ENB 22

// PID constants
float Kp = 23.0;
float Ki = 0.3;
float Kd = 5.0;

// PID variables
float error = 0, lastError = 0, integral = 0, derivative = 0;
float output = 0;

// Motor speeds
int baseSpeedl = 130;
int baseSpeedr = 120 ;
  // Base speed for the motors
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

// Integral windup protection
const float integralMax = 100;  // Maximum allowed integral value
// Finish line detection
bool finishDetected = false;  // Flag to indicate if the finish line is detected


void setup() {
  // Set IR sensor pins as input
  pinMode(IR_LEFT_2, INPUT);
  pinMode(IR_LEFT_1, INPUT);
  pinMode(IR_MIDDLE, INPUT);
  pinMode(IR_RIGHT_1, INPUT);
  pinMode(IR_RIGHT_2, INPUT);

  // Set motor control pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // Read IR sensor values
  int irLeft2 = digitalRead(IR_LEFT_2);
  int irLeft1 = digitalRead(IR_LEFT_1);
  int irMiddle = digitalRead(IR_MIDDLE);
  int irRight1 = digitalRead(IR_RIGHT_1);
  int irRight2 = digitalRead(IR_RIGHT_2);

    // Check if all sensors detect the finish line
  if (irLeft2 == HIGH && irLeft1 == HIGH && irMiddle == HIGH && irRight1 == HIGH && irRight2 == HIGH) {
    finishDetected = true;  // Set the finish flag
  }
  // If finish line is detected, stop the motors
  if (finishDetected) {
    stopMotors();
       Serial.println("Finish line detected! Stopping motors.");
    return;  // Exit the loop to prevent further movement
  }


  // Calculate error based on sensor readings
  error = (irLeft2 * -2) + (irLeft1 * -1) + (irMiddle * 0) + (irRight1 * 1) + (irRight2 * 2);

  // Calculate PID terms
  integral += error;

  // Integral windup protection
  if (integral > integralMax) integral = integralMax;
  if (integral < -integralMax) integral = -integralMax;

  derivative = error - lastError;
  output = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;

  // Adjust motor speeds based on PID output
  leftMotorSpeed = baseSpeedl + output;
  rightMotorSpeed = baseSpeedr - output;

  // Constrain motor speeds to valid PWM range
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  // Set motor directions and speeds
  analogWrite(ENA, leftMotorSpeed);
  analogWrite(ENB, rightMotorSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Debugging output
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" Output: ");
  Serial.print(output); 
  Serial.print(" Left Speed: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" Right Speed: ");
  Serial.println(rightMotorSpeed);

  // Small delay to stabilize the loop
  delay(10);
}

// Function to stop the motors
void stopMotors() {
  analogWrite(ENA, 0);  // Set left motor speed to 0
  analogWrite(ENB, 0);  // Set right motor speed to 0
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,LOW);
}