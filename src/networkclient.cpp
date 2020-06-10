#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include "networkclient.h"
#include "secrets.h"

String mdnsHost;
time_t now;
time_t nowish = 1510592825;


NetworkClient::NetworkClient() {}

NetworkClient::NetworkClient(const String *host)
{
    mdnsHost = *host;
}


void NetworkClient::setTime()
{
    Serial.print("\nSetting time using SNTP");

    configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");

    now = time(nullptr);

    while (now < nowish)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    Serial.print("\nCurrent date/time: ");
    Serial.print(asctime(&timeinfo));
}

void NetworkClient::connect()
{
    delay(1000);

    byte timeout = 50;

    Serial.print("\nConnecting to WiFi network ");
    Serial.print(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    for (int i = 0; i < timeout; i++)
    {
        Serial.print(".");

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.print("\nConnected. IP: ");
            Serial.print(WiFi.localIP());

            if (mdnsHost != nullptr)
            {
                if (MDNS.begin(mdnsHost))
                {
                    Serial.print(" and mDNS: ");
                    Serial.print(mdnsHost);
                    Serial.println(".local");

                    MDNS.addService("http", "tcp", 80);
                }
            }

            setTime();

            return;
        }

        delay(1000);
    }

    Serial.println("Failed to connect to WiFi. Restarting in 2 seconds.");

    delay(2000);
    ESP.restart();
}

void NetworkClient::check()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi connection lost. Reconecting.");

        connect();
    }

    MDNS.update();
}
