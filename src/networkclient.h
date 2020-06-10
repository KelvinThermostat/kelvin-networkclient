#include <Arduino.h>

class NetworkClient
{
private:
    void setTime();

public:
    void connect();
    void check();
    NetworkClient();
    NetworkClient(const String *hostname);
};
