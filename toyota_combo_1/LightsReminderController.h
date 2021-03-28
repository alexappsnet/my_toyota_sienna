#include "PhotoSensor.h"
#include "LightsAlarm.h"

class LightsReminderController {
private:
    PhotoSensor photo_;
    LightsAlarm lightsAlarm_;
    int pinDisable_;
    int pinLightsAlreadyOn_;
    unsigned long lastLightsCheckedAt_;
    int arrow_;
    bool day_;

    bool timeToCheckLights();
    void analyzeDayOrNight(int photoValue);
    bool isCarLightsOn();
    bool isDisableAlarm();

public:
    LightsReminderController(int pinPhoto,
        int pinLightsAlarmLed,
        int pinLightsAlarmBuzzer, int pinDisable,
        int pinLightsAlreadyOn);
    void setup();
    void check();
};
