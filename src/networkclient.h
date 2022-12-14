#include <Arduino.h>

class NetworkClient
{
private:
    void setTime();

public:
    void connect();
    void check();
    String getDateTime();
    void registerMdnsHost(String host);
};
