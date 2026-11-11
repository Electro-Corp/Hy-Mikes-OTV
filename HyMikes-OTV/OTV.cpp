/*
  Hy-Mikes robot system
*/
#include "OTV.h"
#include "direction.h"
#include "speed.h"
#include "motor.h"
#include <Tank.h>

OTV::OTV(){
  isWorking = false;
  currentAction = IDLE;
#ifdef TANK
  Tank.begin();
#endif
  // init color sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);
  pinMode(2, OUTPUT);
  // Output scale 100%
  digitalWrite(S0, HIGH); 
  digitalWrite(S1, HIGH);
  // SHUT OFF LEDs
  digitalWrite(2, LOW); 
  // Voltage reader
  pinMode(VOLT, INPUT);
  // Motor inputs 
  pinMode(MOTOR_1_IN2     , OUTPUT);
  pinMode(MOTOR_2_IN3    , OUTPUT);
  pinMode(MOTOR_2_IN4   , OUTPUT);
  pinMode(MOTOR_1_SPEED , OUTPUT);
  pinMode(MOTOR_2_SPEED , OUTPUT);
  // Motors (3, 4)
  pinMode(MOTOR_3_IN1  , OUTPUT);
  pinMode(MOTOR_3_IN2 , OUTPUT);
  pinMode(MOTOR_4_IN3 , OUTPUT);
  pinMode(MOTOR_4_IN4  , OUTPUT);
  pinMode(MOTOR_3_SPEED , OUTPUT);
  pinMode(MOTOR_4_SPEED, OUTPUT);
}

void OTV::connect(int markerId, int roomNumber){
#ifdef TANK
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber, 52, 50);
#else
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber);
#endif
  Enes100.println("[OTV] Hydrogen Baby connected...");
  transform.angle = -2000;
  transform.x = 0;
  transform.y = 0;
  target.angle = 0;
  updatePosition();

  // Figure out where we are
  Enes100.println("Inital Robot Position:");
  Enes100.print("X: ");
  Enes100.print(transform.x);
  Enes100.print(" Y: ");
  Enes100.print(transform.y);
  Enes100.print(" Theta: ");
  Enes100.print(transform.angle);
  Enes100.println();
  Enes100.print("We are: ");
  Enes100.print(transform.y > 1 ? "Above" : "Below");
  Enes100.println();
}

void OTV::updatePosition(){
  transform.angle = Enes100.getTheta();
  transform.x = Enes100.getX();
  transform.y = Enes100.getY();
}

bool OTV::working(){
  return this->isWorking;
}

// Tick robot to next frame
void OTV::tick(){
  // Check if connection is still active
  if(!Enes100.isConnected()){
    // Guess where we are i guess
  }else{

  }

  // Update the current position data
  if(millis() % 150 == 0) updatePosition();

  // Begin actions
  switch(this->currentAction){
    case TURNING:
      turn_impl();
      break;
    case FORWARD:
      forward_impl();
      break;
    case IDLE:
      if(millis() % 1000 == 0) Enes100.println("[OTV] Idling...");
      break;
    default:
      if(millis() % 1000 == 0) Enes100.println("[OTV] Warning: Current action is some bullshit!");
      break;
  }
}


void OTV::turn(double targetAngle){
  this->currentAction = TURNING;
  this->target.angle = targetAngle;
  this->isWorking = true;
}

void OTV::forward(double distance){
  this->currentAction = FORWARD;
  this->isWorking = true;
  this->prev = transform;
  targetDist = distance;
}

void OTV::strafe(double distance){
  this->currentAction = STRAFE;
  this->isWorking = true;
  this->prev = transform;
  targetDist = distance;
}

// Internal function for rotation for the OTV
void OTV::turn_impl(){
  int turnDirection = 1;
  if(transform.angle - target.angle > 0) turnDirection = -1; 
  double speed = lerp(0, 255, abs(target.angle - transform.angle) / 1.2);
  if(abs(target.angle - transform.angle) <= TURN_MARGIN){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
#ifdef TANK
  //transform.angle += speed * (abs(target.angle - transform.angle) / 255);
  Tank.setRightMotorPWM(speed * turnDirection);
  Tank.setLeftMotorPWM(-speed * turnDirection); 
#else
  // Mecanum
  motorControl(1, speed);
  motorControl(2, -speed);
  motorControl(3, -speed);
  motorControl(4, speed);
#endif
}

// Internal function for rotation for the OTV
void OTV::forward_impl(){
  double distance = dist(transform.x, transform.y, prev.x, prev.y);
  double speed = lerp(0, 255, 1.5 / distance);
  if(distance > (targetDist - POSITION_MARGIN)){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
#ifdef TANK
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(speed); 
#endif
  Enes100.print("forward ");
  Enes100.print(distance);
  Enes100.print(" at ");
  Enes100.print(speed);
  Enes100.print("\n");
  // Mecanum
  motorControl(1, speed);
  motorControl(2, speed);
  motorControl(3, speed);
  motorControl(4, speed);
}

// Internal function for strafeing 
void OTV::strafe_impl(){
  Enes100.print("strafe ");
  Enes100.print(dist(transform.x, transform.y, prev.x, prev.y));
  Enes100.print("\n");
  double distance = dist(transform.x, transform.y, prev.x, prev.y);
  double speed = lerp(0, 255, 1.5 / distance);
  if(distance > (targetDist - POSITION_MARGIN)){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
  // Mecanum
  motorControl(1, speed);
  motorControl(2, -speed);
  motorControl(3, speed);
  motorControl(4, -speed);
}

void OTV::motorControl(int motorNumber, int speed){
  switch(motorNumber){
    case 1: {
      // Set speed and direction
      setMotorSpeed(motorA, speed);
      if(speed < 0) setMotorDirectionBackward(motorA);
      else setMotorDirectionForward(motorA);
      // Send
      sendToMotor(motorA, MOTOR_1_SPEED, MOTOR_1_IN1, MOTOR_1_IN2);
      break;
    }
    case 2: {
      // Set speed and direction
      setMotorSpeed(motorB, speed);
      if(speed < 0) setMotorDirectionBackward(motorB);
      else setMotorDirectionForward(motorB);
      // Send
      sendToMotor(motorB, MOTOR_2_SPEED, MOTOR_2_IN3, MOTOR_2_IN4);
      break;
    }
    case 3: {
      // Set speed and direction
      setMotorSpeed(motorC, speed);
      if(speed < 0) setMotorDirectionBackward(motorC);
      else setMotorDirectionForward(motorC);
      // Send
      sendToMotor(motorC, MOTOR_3_SPEED, MOTOR_3_IN1, MOTOR_3_IN2);
      break;
    }
    case 4: {
      // Set speed and direction
      setMotorSpeed(motorD, speed);
      if(speed < 0) setMotorDirectionBackward(motorD);
      else setMotorDirectionForward(motorD);
      // Send
      sendToMotor(motorD, MOTOR_4_SPEED, MOTOR_4_IN3, MOTOR_4_IN4);
      break;
    }
  }
}

void OTV::readAndPrintColor(){
  // LOW/LOW = RED, LOW/HIGH = BLUE, HIGH/HIGH = GREEN
  // RED
  digitalWrite(S2, LOW);   
  digitalWrite(S3, LOW);
  r = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
  //if(r < 0) r = 0;
  delay(20);  
  // BLUE
  digitalWrite(S3, HIGH);
  b = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);   
  //if(b < 0) b = 0;
  delay(20); 
  // GREEN
  digitalWrite(S2, HIGH);   
  digitalWrite(S3, HIGH);
  g = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);   
  //if(g < 0) g = 0;
  delay(20);  
  //
  double rToG = (double)r/(double)g;
  double rToB = (double)r/(double)b;
  double gToR = 1.0/rToG;
  double gToB = (double)g/(double)b;
  double bToG = 1.0/gToB;
  double bToR = 1.0/rToB;
  
  //
  Enes100.print("Color (r,g,b): (");
  Enes100.print(r);
  Enes100.print(",");
  Enes100.print(g);
  Enes100.print(",");
  Enes100.print(b);
  Enes100.print(") ");
  
  if(rToG < 0.3 && gToB < 0.5) Enes100.print(" YELLOW \n");
  else if(rToG < 0.45 && rToB < 0.45) Enes100.print(" RED \n");
  else if(rToG < 0.8 && rToB < 0.8) Enes100.print(" WHITE \n");
  else if(gToR < 0.5 && gToB < 0.5) Enes100.print(" GREEN \n");
  else if(bToG < 0.7 && bToR < 0.7) Enes100.print(" BLUE \n");
  else Enes100.print(" IDK \n");
  
}

void OTV::readAndPrintVolt(){
  double v = analogRead(VOLT) * (5.0 / 1023.0);
  int vOut = round(v + 0.5);
  Enes100.print("Voltage: ");
  Enes100.print(vOut);
  Enes100.print("\n");
}

bool OTV::isConnected() { return Enes100.isConnected(); }