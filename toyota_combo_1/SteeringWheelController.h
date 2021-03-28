
#include "Toyota_SteeringWheel.h"
#include "Pioneer_Radio.h"

class SteeringWheelController {
private:
    Toyota_SteeringWheel steering_;
    Pioneer_Radio radio_;
public:
    SteeringWheelController(int pinSteering, int pinRadioPot);
    void setup();
    void afterSetup();
    void check();
};
