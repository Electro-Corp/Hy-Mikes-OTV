/*
  Hy-Mikes Hydrogen Baby
*/
#define ARDUINO_UNOR4_WIFI  // IDK if this is needed but im scared to remove it

#include "OTV.h"

typedef enum Stages {
  STAGE_1,
  STAGE_2,
  STAGE_MS5
};

typedef enum STAGE_1_STATES {
  START = 0,
  TURN_TO_OBJECTIVE = 1,
  GOING_FORWARD = 2,
  ALIGN_WITH_CELL = 3,
  READ_SENSORS = 4,
  REVERSE = 5,
  TURN_TO_END = 6,
  DONE = 7
};

typedef enum MS_5_STATES {
  START = 0,
  GOING_FORWARD = 1,
  TURN_1 = 2,
  TURN_2 = 3,
  TURN_3 = 4,
  DONE = 5
};

OTV hBaby;
int currentStage = STAGE_1;
int state = START;
int dir = 1;

// Example "stage 1"
void stage1();
void ms5();

void setup() {
  // Create and init OTV
  hBaby = OTV();
  hBaby.connect(170, 1116);
}

void loop() {
  delay(20);
  // Tick OTV 
  hBaby.tick();
  //hBaby.readAndPrintColor();
  hBaby.readAndPrintVolt();
  // What stage are we on
  switch (currentStage) {
    case STAGE_1:
      // stage1();
      ms5(); // temp for ms5
      break;
    default:
      break;
  }
}

void ms5(){
  if (!hBaby.working()) {
    // Are we connected?
    if(hBaby.getTransform().angle != -2000 && hBaby.isConnected()){
        // Move to next stage
      state += 1;
      delay(1000); // this is specifically for MS5, we can comment it out later
    }
    switch (state) {
      case GOING_FORWARD:
        hBaby.forward(4);
        break;
      case TURN_1:
        hBaby.turn(90);
        break;
      case TURN_2:
        hBaby.turn(180);
        break;
      case TURN_3:
        hBaby.turn(270);
        break;
      case DONE:
        break;
      default:
        break;
    }
  }
}

// Goes forward and then turns and then does nothing
void stage1() {
  if (!hBaby.working()) {
    // Are we connected?
    if(hBaby.getTransform().angle != -2000 && hBaby.isConnected()){
        // Move to next stage
      state += 1;
    }
    switch (state) {
      case TURN_TO_OBJECTIVE:
        if(hBaby.getTransform().y > 1) dir = -1;
        hBaby.turn(1.57);
        break;
      case GOING_FORWARD:
        hBaby.forward(0.8);
        break;
      case ALIGN_WITH_CELL: {
        break;
      }
      case READ_SENSORS:
        hBaby.readAndPrintColor();
        hBaby.readAndPrintVolt();
        break;
      case REVERSE:
        hBaby.forward(-0.8);
      case TURN_TO_END:
        hBaby.turn(0);
      case DONE:
        break;
      default:
        break;
    }
  }
}
