#include <Arduino.h>

class WifiClient
{
private:
    void setTime();

public:
    void connect();
    void check();
    WifiClient();
    WifiClient(const String *hostname);
};
