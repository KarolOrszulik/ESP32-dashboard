#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "time.h"

void NTP_configure(const char* server, long gmtOffset, int dstOffset)
{
    Serial.print("Configuring NTP...");
    configTime(gmtOffset, dstOffset, server);
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        delay(1000);
    }
    Serial.println("done");
}

struct tm NTP_getTime()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}