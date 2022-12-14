#include <Arduino.h>
#include "networkclient.h"

NetworkClient net;

void setup()
{
  Serial.begin(9600);

  net.connect();
  net.registerMdnsHost(F("mDNSHostName"));
}

void loop()
{
  net.check();
}
