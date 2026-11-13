/*
  Hy-Mikes robot system
*/
#ifndef OTV_H
#define OTV_H

#define TURN_MARGIN 0.0314
#define POSITION_MARGIN 0.05

//#define TANK                // Define tank so OTV knows we're in tank mode (for now...)

#include <Enes100.h>

// Color Sensor
#define S0 8        // we need to reassign pins cuz overlap
#define S1 5
#define S2 7
#define S3 6
#define COLOR_OUT 12
// Voltage
#define VOLT A5
// Motors (1, 2)
#define MOTOR_1_IN1         7
#define MOTOR_1_IN2         5
#define MOTOR_2_IN3         11
#define MOTOR_2_IN4         12
#define MOTOR_1_SPEED       6
#define MOTOR_2_SPEED       10
// Motors (3, 4) 
#define MOTOR_3_IN1         13
#define MOTOR_3_IN2         8
#define MOTOR_4_IN3         4
#define MOTOR_4_IN4         2
#define MOTOR_3_SPEED       9
#define MOTOR_4_SPEED       3


typedef struct Transform{
  double x, y, angle;
};

typedef struct motor {   
	 byte speed = 0;   
	 struct {   
	   byte input1 = LOW;   
	   byte input2 = LOW;   
	 } direction;   
};   

typedef enum Action{
  IDLE = 0,
  TURNING = 1,
  FORWARD = 2,
  STRAFE = 3,
};

class OTV{
private:
  Transform transform, target, prev;
  double targetDist;
  bool isWorking;
  Action currentAction;
  // Color
  int r,g,b;
  // Motors 
  //     TL      TR      BR      BL
  motor motorA, motorB, motorC, motorD;

  // Internal
  void turn_impl();
  void forward_impl();
  void strafe_impl();

  // Helpful
  static double lerp(const double v0, const double v1, const double t) {
    return (1 - t) * v0 + t * v1;
  }
  static double dist(const double x1, const double y1, const double x2, const double y2){
    return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
  }
public:
  OTV();

  // Connect to the vision system
  void connect(int markerId, int roomNumber);
  // Update internal position
  void updatePosition();
  // Tick robot to next frame
  void tick();
  // Still completing previous task?
  bool working();

    // Motor control
  void motorControl(int motorNumber, int speed);

  // 
  // Movement
  // 
  // Turn to [targetAngle]
  void turn(double targetAngle);
  // Move forward [distance] in the direction we are facing
  void forward(double distance);
  // Strafe left or right (direction determined by sign and current direction)
  void strafe(double distance);


  //
  // Sensors
  //
  void readAndPrintColor();
  void readAndPrintVolt();

  // Getters
  Transform getTransform(){ return transform; }
  bool isConnected();
};

#endif