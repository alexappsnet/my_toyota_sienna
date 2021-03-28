
/* MCP4151-104E, 100 kOhm, 257 steps */

class MCP4151_104E {
private:
    int pin_;
    bool isConnected_;
public:
    MCP4151_104E(int pin);
    void setup();
    void connect();
    void disconnect();
    void set(int value_0_256);
};
