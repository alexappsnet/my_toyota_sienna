
#include "MCP4151_104E.h"

class Pioneer_Radio {
private:
    MCP4151_104E pot_;
    int prevCommand_;
    void enableResistor(int steps);
public:
    Pioneer_Radio(int potPin);
    void setup();
    void afterSetup();
    void sendCommand(int command);
};
