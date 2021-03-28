#include "LightsReminderController.h"
#include <Arduino.h>

#define SENSOR_TOO_DARK 200
#define SENSOR_DAY_LIGHT 300

#define ARROW_NIGHT_VALUE 0
#define ARROW_DAY_VALUE 40

bool isTooDarkSensor(int photoValue) {
  return photoValue <= SENSOR_TOO_DARK;
}

bool isDayLightSensor(int photoValue) {
  return photoValue >= SENSOR_DAY_LIGHT;
}

LightsReminderController::LightsReminderController(int pinPhoto, int pinLightsAlarmLed,
    int pinLightsAlarmBuzzer, int pinDisable,
    int pinLightsAlreadyOn)
    : photo_(pinPhoto), lightsAlarm_(pinLightsAlarmLed, pinLightsAlarmBuzzer) {
    lastLightsCheckedAt_ = 0;
    arrow_ = ARROW_DAY_VALUE;
    pinDisable_ = pinDisable;
    pinLightsAlreadyOn_ = pinLightsAlreadyOn;
    day_ = true;
}

void LightsReminderController::setup() {
    photo_.setup();
    lightsAlarm_.setup();
    pinMode(pinDisable_, INPUT_PULLUP);
    pinMode(pinLightsAlreadyOn_, INPUT_PULLUP);
}

bool LightsReminderController::timeToCheckLights() {
  unsigned long now = millis();
  unsigned long passed = now - lastLightsCheckedAt_;
  if (passed >= 250 /*ms*/) {
    lastLightsCheckedAt_ = now;
    return true;
  } else {
    return false;
  }  
}

bool LightsReminderController::isCarLightsOn() {
  return digitalRead(pinLightsAlreadyOn_) == LOW;
}

bool LightsReminderController::isDisableAlarm() {
  return digitalRead(pinDisable_) == LOW;
}

void LightsReminderController::analyzeDayOrNight(int photoValue) {
  if (isTooDarkSensor(photoValue))
    arrow_ -= 1;

  if (isDayLightSensor(photoValue))
    arrow_ += 2; // Move the arrow faster from night to day: += 2

  if (isDisableAlarm() || isCarLightsOn())
    arrow_ = ARROW_DAY_VALUE;

  if (arrow_ >= ARROW_DAY_VALUE) {
    arrow_ = ARROW_DAY_VALUE;
    day_ = true;
  }

  if (arrow_ <= ARROW_NIGHT_VALUE) {
    arrow_ = ARROW_NIGHT_VALUE;
    day_ = false;
  }
}

void LightsReminderController::check() {
  if (!timeToCheckLights())
    return;

  analyzeDayOrNight(photo_.readSensor());

  if (day_)
    lightsAlarm_.alarmOff();
  else
    lightsAlarm_.alarmTickOn();
}
