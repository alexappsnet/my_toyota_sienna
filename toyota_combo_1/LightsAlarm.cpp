#include "LightsAlarm.h"
#include <Arduino.h>

LightsAlarm::LightsAlarm(int pinLed, int pinBuzzer) {
    pinLed_ = pinLed;
    pinBuzzer_ = pinBuzzer;
    alarmCount_ = 0;
}

void LightsAlarm::setup() {
  pinMode(pinLed_, OUTPUT);
  pinMode(pinBuzzer_, OUTPUT);
}

void LightsAlarm::alarmWithLED() {
  int led = LOW;
  if (alarmCount_ == 0 || alarmCount_ == 1
      || alarmCount_ == 5 || alarmCount_ == 9)
    led = HIGH;
  digitalWrite(pinLed_, led);
}

void LightsAlarm::alarmWithTone() {
  if (alarmCount_ == 0)
    tone(pinBuzzer_, 1976);
  else if (alarmCount_ == 1)
    tone(pinBuzzer_, 1760);
  else
    noTone(pinBuzzer_);
}

void LightsAlarm::alarmTickOn() {
  alarmWithLED();
  alarmWithTone();

  if (++alarmCount_ >= 40)
    alarmCount_ = 0;
}

void LightsAlarm::alarmOff() {
  digitalWrite(pinLed_, LOW);
  noTone(pinBuzzer_);
  alarmCount_ = 0;
}
