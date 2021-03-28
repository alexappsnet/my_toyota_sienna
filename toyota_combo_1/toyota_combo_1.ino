
#include "SteeringWheelController.h"
#include "LightsReminderController.h"

#include <SPI.h>

#define PIN_STEERING_WHEEL A0
#define PIN_RESISTOR 4

#define PIN_PHOTO A4

#define PIN_LIGHTS_ALARM_LED 3
#define PIN_LIGHTS_ALARM_BUZZER 6

#define PIN_DISABLE_ALARM 8
#define PIN_CAR_LIGTHS_ON 7

SteeringWheelController steeringWheelController(
                  PIN_STEERING_WHEEL,
                  PIN_RESISTOR);

LightsReminderController lightsReminderController(
                  PIN_PHOTO,
                  PIN_LIGHTS_ALARM_LED,
                  PIN_LIGHTS_ALARM_BUZZER,
                  PIN_DISABLE_ALARM,
                  PIN_CAR_LIGTHS_ON);

void setup() {
  steeringWheelController.setup();
  lightsReminderController.setup();
  SPI.begin();
  steeringWheelController.afterSetup();
}

void loop() {
  steeringWheelController.check();
  lightsReminderController.check();
}
