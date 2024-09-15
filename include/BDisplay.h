#pragma once

#define PxMATRIX_DOUBLE_BUFFER true
#include <PxMatrix.h>

constexpr byte P_LAT = 22;
constexpr byte P_A   = 19;
constexpr byte P_B   = 23;
constexpr byte P_C   = 18;
constexpr byte P_D   = 5;
constexpr byte P_E   = 15;
constexpr byte P_OE  = 16;
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

PxMATRIX display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

uint16_t led_time = 20;
uint16_t color = 0xFFFF;

void IRAM_ATTR display_updater()
{
    portENTER_CRITICAL_ISR(&timerMux);
    display.display(led_time);
    portEXIT_CRITICAL_ISR(&timerMux);
}

void Display_init()
{
    display.begin(32);
    display.setColorOrder(BBRRGG);
    display.setBrightness(8);
    display.clearDisplay();
    display.showBuffer();
}

void Display_createRefreshTask()
{
    xTaskCreatePinnedToCore(
        [] (void*) {
            timer = timerBegin(0, 80, true); // clock divided by 80 = 1MHz -> T=1us
            timerAttachInterrupt(timer, &display_updater, true);
            timerAlarmWrite(timer, 2000, true); // trigger every 2000us
            timerAlarmEnable(timer);
            while (true) {
                delay(1000); // Keep the task alive
            }
        },
        "DisplayTask",  // Task name
        10000,          // Stack size
        NULL,           // Task parameter
        1,              // Task priority
        NULL,           // Task handle
        1               // Core to run the task on
    );
}
