#include "Pioneer_Radio.h"
#include "CustomCommand.h"
#include <Arduino.h>

Pioneer_Radio::Pioneer_Radio(int potPin): pot_(potPin) {
    prevCommand_ = COMMAND_NONE;
}

void Pioneer_Radio::setup() {
    pot_.setup();
}

void Pioneer_Radio::afterSetup() {
    pot_.disconnect();
}

void Pioneer_Radio::enableResistor(int steps) {
    pot_.set(steps);
    pot_.connect();
}

void Pioneer_Radio::sendCommand(int command) {
    if (prevCommand_ == command)
        return;

    prevCommand_ = command;

    pot_.disconnect();

    switch(command) {
        case COMMAND_MODE: // 1200
            enableResistor(2); // att and long press - voice
            break;
        case COMMAND_CHANNEL_UP: // 8000
            enableResistor(18);
            break;
        case COMMAND_CHANNEL_DOWN: // 11250
            enableResistor(26);
            break;
        case COMMAND_VOLUME_UP: // 16000
            enableResistor(38);
            break;
        case COMMAND_VOLUME_DOWN: // 24000
            enableResistor(58);
            break;
    }

    if (command != COMMAND_NONE)
        delay(50);
}
