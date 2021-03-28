#include "MCP4151_104E.h"
#include <Arduino.h>
#include <SPI.h>

/*
 * Connect/disconnect the R0W
 */
void setPotOnOff(int potPin, bool on) {
  digitalWrite(potPin, LOW);
  delay(1);
  
  SPI.transfer(B01000001);

  if (on)
    SPI.transfer(B11111111);
  else
    SPI.transfer(B11011101);

  delay(1);
  digitalWrite(potPin, HIGH);
  delay(1);
}

void setPot(int potPin, int steps) {
  digitalWrite(potPin, LOW);
  delay(1);
  SPI.transfer(B00000000 | ((steps >> 8) & 0x01));
  SPI.transfer(steps & 0xFF);
  delay(1);
  digitalWrite(potPin, HIGH);
  delay(1);
}

MCP4151_104E::MCP4151_104E(int pin) {
    pin_ = pin;
    isConnected_ = true;
}

void MCP4151_104E::setup() {
    pinMode(pin_, OUTPUT);
}

void MCP4151_104E::connect() {
  setPotOnOff(pin_, true);
}
    
void MCP4151_104E::disconnect() {
  setPotOnOff(pin_, false);
}

void MCP4151_104E::set(int value_0_256) {
  setPot(pin_, value_0_256);
}
