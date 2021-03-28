#include "Toyota_SteeringWheel.h"
#include "CustomCommand.h"
#include <Arduino.h>

// Read the value 2 times with 25ms delay.
// If the reads are the same or very close,
// assume it is a good read.
int readCarVoltage(int pin) {
  long voltage1 = analogRead(pin);
  while (1) {
    delay(25);
    long voltage2 = analogRead(pin);
    if (abs(voltage2 - voltage1) < 5 )
      return (int) ((voltage1 + voltage2) / 2);
    voltage1 = voltage2;
  }
}

int guessCurrentButton(long carVoltage) {
  if (abs(40 - carVoltage) <= 40)
    return COMMAND_CHANNEL_UP;
  
  if (abs(270 - carVoltage) <= 40)
    return COMMAND_CHANNEL_DOWN;
  
  if (abs(380 - carVoltage) <= 40)
    return COMMAND_MODE;

  if (abs(523 - carVoltage) <= 40)
    return COMMAND_VOLUME_UP;

  if (abs(770 - carVoltage) <= 40)
    return COMMAND_VOLUME_DOWN;

  return COMMAND_NONE;
}

Toyota_SteeringWheel::Toyota_SteeringWheel(int analogPin) {
    analogPin_ = analogPin;
}

void Toyota_SteeringWheel::setup() {
    pinMode(analogPin_, INPUT);
}

int Toyota_SteeringWheel::readButtonCommand() {
    return guessCurrentButton(readCarVoltage(analogPin_));
}
