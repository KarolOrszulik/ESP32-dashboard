#pragma once

#include <Arduino.h>
#include <time.h>

extern struct tm g_timeInfo;

void NTP_configure(const char* server, long gmtOffset, int dstOffset);
struct tm NTP_getTime();
bool NTP_isNewSecond(int currentSecond);