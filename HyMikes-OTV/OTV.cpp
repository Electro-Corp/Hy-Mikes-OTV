/*
  Hy-Mikes robot system
*/
#include "OTV.h"
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

// Internal function for rotation for the OTV
void OTV::turn_impl(){
  // Turn logic for Tank, will be different for real robot
#ifdef TANK
  int turnDirection = 1;
  if(transform.angle - target.angle > 0) turnDirection = -1; 
  double speed = lerp(0, 255, abs(target.angle - transform.angle) / 1.2);
  if(abs(target.angle - transform.angle) <= TURN_MARGIN){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
  //transform.angle += speed * (abs(target.angle - transform.angle) / 255);
  Tank.setRightMotorPWM(speed * turnDirection);
  Tank.setLeftMotorPWM(-speed * turnDirection); 
#endif
}

// Internal function for rotation for the OTV
void OTV::forward_impl(){
#ifdef TANK
  Enes100.print("forward ");
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
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(speed); 
#endif
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
  // GREEWN
  digitalWrite(S2, HIGH);   
  digitalWrite(S3, HIGH);
  g = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);   
  //if(g < 0) g = 0;
  delay(20);  
  //
  Enes100.print("Color (r,g,b): (");
  Enes100.print(r);
  Enes100.print(",");
  Enes100.print(g);
  Enes100.print(",");
  Enes100.print(b);
  Enes100.print(") ");
  if(/*r < 100 && g > 100 && b > 200*/ b < (g+2000)) Enes100.print(" BLUE \n");
  else if(/*r > 150 && g < 50 && b < 10*/ r < (g+1000)) Enes100.print(" RED \n");
  else if(/*r < 200 && g > 200 && b < 200*/ g < (b+800) && g < (r+800)) Enes100.print(" GREEN \n");
  else if(r < (g+40) || g < (r+40) && g < (b+40) || b < (g+40) && b < (r+40) || r < (b+40)) Enes100.print(" WHITE \n");
  else if(r > (g+100) && g > (b+50)) Enes100.print(" YELLOW \n");
  else Enes100.print(" IDK \n");
  
}

bool OTV::isConnected() { return Enes100.isConnected(); }