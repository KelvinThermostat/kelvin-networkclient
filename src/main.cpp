#include <Arduino.h>
#include "networkclient.h"

const String host = "mDNShost";

NetworkClient net = NetworkClient(&host);

void setup()
{
  Serial.begin(9600);

  net.connect();
}

void loop()
{
  net.check();
}
