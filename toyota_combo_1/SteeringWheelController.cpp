#include "SteeringWheelController.h"
#include <Arduino.h>

SteeringWheelController::SteeringWheelController(int pinSteering, int pinRadioPot)
    : steering_(pinSteering), radio_(pinRadioPot) {
}

void SteeringWheelController::setup() {
  steering_.setup();
  radio_.setup();
}

void SteeringWheelController::afterSetup() {
  radio_.afterSetup();
}

void SteeringWheelController::check() {
  int carButtonCommand = steering_.readButtonCommand();
  radio_.sendCommand(carButtonCommand);
}
