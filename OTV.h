/*
  Hy-Mikes robot system
*/
#ifndef OTV_H
#define OTV_H

#define TURN_MARGIN 0.0314

#ifdef TANK
#include <Tank.h>
#endif

#include <Enes100.h>

typedef struct Transform{
  double x, y, angle;
};

typedef enum Action{
  TURNING = 0,
  FORWARD = 1,
  STRAFE = 2,
  IDLE = 3
};

class OTV{
private:
  Transform transform, target;
  bool isWorking;
  Action currentAction;

  // Internal
  void turn_impl();
  void forward_impl();

  // Helpful
  static double lerp(const double v0, const double v1, const double t) {
    return (1 - t) * v0 + t * v1;
  }
public:
  OTV();

  // Connect to the vision system
  void connect(int markerId, int roomNumber);
  // Tick robot to next frame
  void tick();
  // Still completing previous task?
  bool working();

  //
  // Movement
  void turn(double targetAngle);
  void forward(double distance);
};

#endif