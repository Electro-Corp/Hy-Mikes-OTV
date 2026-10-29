/*
  Hy-Mikes Hydrogen Baby
*/
#define ARDUINO_UNOR4_WIFI  // IDK if this is needed but im scared to remove it
#define TANK 1              // Define tank so OTV knows we're in tank mode (for now...)

#include "OTV.h"

typedef enum Stages {
  STAGE_1,
  STAGE_2
}

typedef enum ExampleStates {
  IDLE = 0,
  GOING_FORWARD = 1,
  TURN_90 = 2,
  IDLE = 3
};

OTV hBaby;
Stages currentStage = STAGE_1;
ExampleStates state = IDLE;

// Example "stage 1"
void stage1();

void setup() {
  // put your setup code here, to run once:
  hBaby = OTV();
  hBaby.connect(15, 1160);
}

void loop() {
  // put your main code here, to run repeatedly:
  hBaby.tick();
  switch (currentStage) {
    case STAGE_1:
      stage1();
      break;
    default:
      break;
  }
}

void stage1() {
  if (!hBaby.working()) {
    // Move to next stage
    state++;
    switch (state) {
      case GOING_FORWARD:
        hBaby.forward(1);
        break;
      case ROTATE_90:
        double a = hBaby.getTransform().angle;
        hBaby.turn(1.5708);
        break;
      default:
        break;
    }
  }
}
