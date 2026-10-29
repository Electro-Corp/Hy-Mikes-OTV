/*
  Hy-Mikes robot system
*/
#include "OTV.h"

OTV::OTV(){
#ifdef TANK
  Tank.begin();
#endif
}

void OTV::connect(int markerId, int roomNumber){
#ifdef TANK
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber, 52, 50);
#else
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber);
#endif
  Enes100.println("[OTV] Hydrogen Baby connected...");
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
    // Update on some tick
    if(millis() % 500 == 0){
      updatePosition();
    }
  }

  // Begin actions
  switch(this->currentAction){
    case TURNING:
      turn_impl();
      break;
    case FORWARD:
      forward_impl();
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
  // Calculate target X,Y
  double tX = cos(this->transform.angle) * distance;
  double tY = cos(this->transform.angle) * distance;
  this->target.x = this->transform.x + tX;
  this->target.y = this->transform.y + tY;
}

// Internal function for rotation for the OTV
void OTV::turn_impl(){
  // Turn logic for Tank, will be different for real robot
#ifdef TANK
  int turnDirection = 1;
  if(transform.angle - target.angle > 0) turnDirection = -1; 
  double speed = lerp(0, 255, abs(target.angle - transform.angle) / 1.1);
  if(abs(target.angle - transform.angle) <= TURN_MARGIN){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
  transform.angle += speed * (abs(target.angle - transform.angle) / 255);
  Tank.setRightMotorPWM(speed * dir);
  Tank.setLeftMotorPWM(-speed * dir); 
#endif
}

// Internal function for rotation for the OTV
void OTV::forward_impl(){
#ifdef TANK
  double distance = dist(transform.x, transform.y, target.x, target.y);
  double speed = lerp(0, 255, distance / 1.5);
  if(distance < POSITION_MARGIN){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
  transform.position.x += cos(this->transform.angle);
  transform.position.y += sin(this->transform.angle);
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(speed); 
#endif
}