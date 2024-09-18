#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

void WiFi_connect(const char* ssid, const char* password, const char* hostname, uint32_t timeout = 10000U);