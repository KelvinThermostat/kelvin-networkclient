#include <Arduino.h>
#include "wificlient.h"

const String host = "mDNShost";

WifiClient wifi = WifiClient(&host);

void setup()
{
  Serial.begin(9600);

  wifi.connect();
}

void loop()
{
  wifi.check();
}
