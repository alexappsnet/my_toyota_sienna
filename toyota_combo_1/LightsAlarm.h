
class LightsAlarm {
private:
    int pinLed_;
    int pinBuzzer_;
    int alarmCount_;
    void alarmWithLED();
    void alarmWithTone();
public:
    LightsAlarm(int pinLed, int pinBuzzer);
    void setup();
    void alarmTickOn();
    void alarmOff();
};
