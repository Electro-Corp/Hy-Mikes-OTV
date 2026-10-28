/*
  Hy-Mikes Hydrogen Baby
*/


#define ARDUINO_UNOR4_WIFI // IDK if this is needed but im scared to remove it
#define TANK 1 // Define tank so OTV knows we're in tank mode

#include "OTV.h"

OTV hBaby;

void setup() {
  // put your setup code here, to run once:
  hBaby = OTV();
}

void loop() {
  // put your main code here, to run repeatedly:
  hBaby.tick();
}
