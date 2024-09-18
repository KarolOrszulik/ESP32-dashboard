#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

extern WebServer g_webServer;

void WebServer_setCallbacks();
