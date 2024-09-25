#include "FDisplay.h"

constexpr byte P_LAT = 22;
constexpr byte P_A   = 5;
constexpr byte P_B   = 18;
constexpr byte P_C   = 23;
constexpr byte P_D   = 19;
constexpr byte P_E   = 15;
constexpr byte P_OE  = 16;

hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

PxMATRIX g_display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);


uint16_t g_ledDuration = 40;
uint16_t g_colorPrimary = g_display.color565(255, 20, 0);
uint16_t g_colorSecondary = g_display.color565(0, 0, 64);

void IRAM_ATTR display_updater()
{
    portENTER_CRITICAL_ISR(&timerMux);
    g_display.display(g_ledDuration);
    portEXIT_CRITICAL_ISR(&timerMux);
}

void Display_createRefreshTask()
{
    xTaskCreatePinnedToCore(
        [] (void*) {
            timer = timerBegin(0, 80, true); // clock divided by 80 = 1MHz -> T=1us
            timerAttachInterrupt(timer, &display_updater, true);
            timerAlarmWrite(timer, 4000, true);
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

void Display_init()
{
    g_display.begin(32);
    g_display.setColorOrder(BBRRGG);
    g_display.setBrightness(16);
    g_display.clearDisplay();
    g_display.showBuffer();

    Display_createRefreshTask();
}