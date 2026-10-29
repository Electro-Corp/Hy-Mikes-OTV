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
}

void OTV::connect(int markerId, int roomNumber){
#ifdef TANK
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber, 52, 50);
#else
  Enes100.begin("Hy-Mikes", HYDROGEN, markerId, roomNumber);
#endif
  Enes100.println("[OTV] Hydrogen Baby connected...");
  transform.angle = 0;
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
    // Update on some tick
    if(millis() % 500 == 0){
      updatePosition();
    }
  }

  updatePosition();

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
  Enes100.print("our angle: ");
  Enes100.print(transform.angle);
  Enes100.print(" and we wanna go to: ");
  Enes100.print(target.angle);
  Enes100.print("\n");
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
  if(distance > targetDist){
    // We are in margin
    this->isWorking = false;
    this->currentAction = IDLE;
    speed = 0;
  }
  Tank.setRightMotorPWM(speed);
  Tank.setLeftMotorPWM(speed); 
#endif
}

bool OTV::isConnected() { return Enes100.isConnected(); }