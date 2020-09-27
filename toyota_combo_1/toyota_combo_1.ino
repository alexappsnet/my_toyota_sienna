
#include <SPI.h>

#define PIN_PHOTO A4

#define PIN_LED 3
#define PIN_CAR_LIGTHS_ON 7
#define PIN_DISABLE_ALARM 8

#define PIN_CAR A0
#define PIN_RESISTOR 4 /* MCP4151-104E, 100 kOhm, 257 steps */
#define PIN_RADIO_SWITCH 9 /* MOC3021 */

#define SENSOR_TOO_DARK 200
#define SENSOR_DAY_LIGHT 300
#define ARROW_NIGHT_VALUE 0
#define ARROW_DAY_VALUE 40

#define COMMAND_NONE 0
#define COMMAND_VOLUME_UP 1
#define COMMAND_VOLUME_DOWN 2
#define COMMAND_CHANNEL_UP 3
#define COMMAND_CHANNEL_DOWN 4
#define COMMAND_MODE 5

bool day = true;
int arrow = ARROW_DAY_VALUE;
int alarmCount = 0;
unsigned long prevLightsCheck = 0;

int prevButton = -1;

void setupSteeingWheelPins() {
  pinMode(PIN_CAR, INPUT);
  pinMode(PIN_RESISTOR, OUTPUT);
  pinMode(PIN_RADIO_SWITCH, OUTPUT);
  digitalWrite(PIN_RADIO_SWITCH, LOW);  
}

// Read the value 2 times with 25ms delay.
// If the reads are the same or very close,
// assume it is a good read.
long readCarVoltage() {
  long voltage1 = analogRead(PIN_CAR);
  while (1) {
    delay(25);
    long voltage2 = analogRead(PIN_CAR);
    if (abs(voltage2 - voltage1) < 5 )
      return (voltage1 + voltage2) / 2;
    voltage1 = voltage2;
  }
}

int guessCurrentButton(long car_voltage) {
  if (abs(40 - car_voltage) <= 40)
    return COMMAND_CHANNEL_UP;
  
  if (abs(270 - car_voltage) <= 40)
    return COMMAND_CHANNEL_DOWN;
  
  if (abs(380 - car_voltage) <= 40)
    return COMMAND_MODE;

  if (abs(523 - car_voltage) <= 40)
    return COMMAND_VOLUME_UP;

  if (abs(770 - car_voltage) <= 40)
    return COMMAND_VOLUME_DOWN;

  return COMMAND_NONE;
}

void setResistorForRadio(int steps) {
  digitalWrite(PIN_RESISTOR, LOW);
  SPI.transfer(0);
  SPI.transfer(steps);
  digitalWrite(PIN_RESISTOR, HIGH);
  delay(20);
  digitalWrite(PIN_RADIO_SWITCH, HIGH);
}

void sendHU(int button) {
  if (prevButton == button)
    return;
  prevButton = button;

  digitalWrite(PIN_RADIO_SWITCH, LOW);
  delay(20);

  switch(button) {
    case COMMAND_MODE: // 1200
      setResistorForRadio(2); // att and long press - voice
      break;
    case COMMAND_CHANNEL_UP: // 8000
      setResistorForRadio(18);
      break;
    case COMMAND_CHANNEL_DOWN: // 11250
      setResistorForRadio(26);
      break;
    case COMMAND_VOLUME_UP: // 16000
      setResistorForRadio(38);
      break;
    case COMMAND_VOLUME_DOWN: // 24000
      setResistorForRadio(58);
      break;
  }
}

void processSteeringWheel() {
  long car_voltage = readCarVoltage();
  int car_button = guessCurrentButton(car_voltage);
  
  sendHU(car_button);
  if (car_button != COMMAND_NONE) {
    delay(50);
  }
}

void alarmOff() {
  digitalWrite(PIN_LED, LOW);
  alarmCount = 0;
}

void alarmWithLED() {
  if (alarmCount == 0 || alarmCount == 1
      || alarmCount == 5 || alarmCount == 9)
  {
    digitalWrite(PIN_LED, HIGH);
  }
  else
  {
    digitalWrite(PIN_LED, LOW);
  }
}

void alarmOn() {
  alarmWithLED();

  if (++alarmCount >= 40)
    alarmCount = 0;
}

bool isCarLightsOn() {
  return digitalRead(PIN_CAR_LIGTHS_ON) == LOW;
}

bool isDisableAlarm() {
  return digitalRead(PIN_DISABLE_ALARM) == LOW;
}

bool isTooDarkSensor(int photoValue) {
  return photoValue <= SENSOR_TOO_DARK;
}

bool isDayLightSensor(int photoValue) {
  return photoValue >= SENSOR_DAY_LIGHT;
}

void analyzeDayOrNight(int photoValue) {
  if (isTooDarkSensor(photoValue))
    arrow -= 1;

  if (isDayLightSensor(photoValue))
    arrow += 2; // Move the arrow faster from night to day: += 2

  if (isDisableAlarm() || isCarLightsOn())
    arrow = ARROW_DAY_VALUE;

  if (arrow >= ARROW_DAY_VALUE) {
    arrow = ARROW_DAY_VALUE;
    day = true;
  }

  if (arrow <= ARROW_NIGHT_VALUE) {
    arrow = ARROW_NIGHT_VALUE;
    day = false;
  }
}

int readLightSensor() {
  return analogRead(PIN_PHOTO);
}

bool timeToCheckLight() {
  unsigned long now = millis();
  if (now - prevLightsCheck >= 250 /*ms*/) {
    prevLightsCheck = now;
    return true;
  } else {
    return false;
  }  
}

void checkAutolight() {
  if (!timeToCheckLight())
    return;

  analyzeDayOrNight(readLightSensor());

  if (day)
    alarmOff();
  else
    alarmOn();
}

void setupAutolight() {
  pinMode(PIN_PHOTO, INPUT);
  pinMode(PIN_CAR_LIGTHS_ON, INPUT);
  pinMode(PIN_DISABLE_ALARM, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
}

void setup() {
  SPI.begin();
  setupSteeingWheelPins();
  setupAutolight();
}

void loop() {
  checkAutolight();
  processSteeringWheel();
}
