#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "networkclient.h"
#include "secrets.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String mdnsHost;

void NetworkClient::registerMdnsHost(String host)
{
    mdnsHost = host;

    if (MDNS.begin(mdnsHost))
    {
        Serial.print(F("\nmDNS: "));
        Serial.print(mdnsHost);
        Serial.println(F(".local"));

        MDNS.addService("http", "tcp", 80);
    }
}

void NetworkClient::setTime()
{
    Serial.print(F("\nSetting time using SNTP"));

    timeClient.begin();
    timeClient.setTimeOffset(0); // GMT
    timeClient.update();

    time_t epochTime = timeClient.getEpochTime();

    struct tm *timeinfo = gmtime((time_t *)&epochTime);

    Serial.print(F("\nCurrent date/time: "));
    Serial.print(asctime(timeinfo));
}

void NetworkClient::connect()
{
    delay(1000);

    byte timeout = 50;

    Serial.print(F("\nConnecting to WiFi network "));
    Serial.print(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    for (int i = 0; i < timeout; i++)
    {
        Serial.print(".");

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.print(F("\nConnected. IP: "));
            Serial.print(WiFi.localIP());

            setTime();

            return;
        }

        delay(1000);
    }

    Serial.println(F("Failed to connect to WiFi. Restarting in 2 seconds"));

    delay(2000);
    ESP.restart();
}

void NetworkClient::check()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Wifi connection lost. Reconecting."));

        connect();
    }

    if (mdnsHost != nullptr)
    {
        MDNS.update();
    }

    timeClient.update();
}

String NetworkClient::getDateTime()
{
    return timeClient.getFormattedTime();
}
