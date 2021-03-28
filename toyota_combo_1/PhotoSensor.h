class PhotoSensor {
private:
    int pin_;
public:
    PhotoSensor(int pin);
    void setup();
    int readSensor();
};
