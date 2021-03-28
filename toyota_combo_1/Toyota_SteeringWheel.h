
class Toyota_SteeringWheel {
private:
    int analogPin_;
public:
    Toyota_SteeringWheel(int analogPin);
    void setup();
    int readButtonCommand();
};
