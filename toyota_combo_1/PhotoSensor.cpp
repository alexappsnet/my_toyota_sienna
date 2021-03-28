#include "PhotoSensor.h"
#include <Arduino.h>

PhotoSensor::PhotoSensor(int pin) {
    pin_ = pin;
}

void PhotoSensor::setup() {
  pinMode(pin_, INPUT);
}

int PhotoSensor::readSensor() {
  return analogRead(pin_);
}
