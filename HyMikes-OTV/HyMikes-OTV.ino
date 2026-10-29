/*
  Hy-Mikes Hydrogen Baby
*/
#define ARDUINO_UNOR4_WIFI  // IDK if this is needed but im scared to remove it

#include "OTV.h"

typedef enum Stages {
  STAGE_1,
  STAGE_2
};

typedef enum ExampleStates {
  START = 0,
  GOING_FORWARD = 1,
  TURN_90 = 2,
  DONE = 3
};

OTV hBaby;
int currentStage = STAGE_1;
int state = START;

// Example "stage 1"
void stage1();

void setup() {
  // Create and init OTV
  hBaby = OTV();
  hBaby.connect(170, 1116);
}

void loop() {
  // Tick OTV 
  hBaby.tick();
  // What stage are we on
  switch (currentStage) {
    case STAGE_1:
      stage1();
      break;
    default:
      break;
  }
}

// Goes forward and then turns and then does nothing
void stage1() {
  if (!hBaby.working()) {
    // Move to next stage
    if(hBaby.getTransform().angle != 0 && hBaby.isConnected()) state += 1;
    switch (state) {
      case GOING_FORWARD:
        hBaby.forward(1);
        break;
      case TURN_90: {
        double a = hBaby.getTransform().angle;
        hBaby.turn(1.5708);
        break;
      }
      case DONE:
        break;
      default:
        break;
    }
  }
}
