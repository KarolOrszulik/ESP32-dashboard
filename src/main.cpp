#include <FDisplay.h>
#include <FWiFi.h>
#include <FNTP.h>
#include <FWebServer.h>

#include <NetworkVariable.h>
#include <Frame_Time.h>
#include <Frame_Date.h>
#include <Frame_NetworkVariable.h>


NetworkVariable varLeft{"left"};
NetworkVariable varRight{"right"};

Frame_NetworkVariable tempFrame({2, 1}, {29, 17}, &varLeft);
Frame_NetworkVariable humidFrame({33, 1}, {29, 17}, &varRight);
Frame_Time clockFrame({1, 20}, {62, 24});
Frame_Date dateFrame({2, 46}, {60, 17});

Frame *frames[] = {&tempFrame, &humidFrame, &clockFrame, &dateFrame};

NetworkVariable varRadius{"radius"};

#define NUM_POINTS 180
struct Point {int x; int y;};
Point points[NUM_POINTS] = {
    {0, -32}, {1, -32}, {2, -32}, {3, -32}, {4, -32}, {5, -32},
    {6, -31}, {7, -31}, {8, -31}, {9, -31},
    {10, -30}, {11, -30},
    {12, -29}, {13, -29}, {14, -29},
    {15, -28},
    {16, -27}, {17, -27},
    {18, -26},
    {19, -25}, {20, -25}, 
    {21, -24},
    {22, -23}
};

void setup()
{
    Serial.begin(115200);
    delay(500);

#include "secret_wifi_credentials.h"
    WiFi_connect(WIFI_SSID, WIFI_PASSWORD, "esp32dash");

    WebServer_setCallbacks();
    g_webServer.begin();

#include "ntp_settings.h"
    NTP_configure(NTP_SERVER, GMT_OFFSET_S, DST_OFFSET_S);

    Display_init();

    varLeft.init();
    varRight.init();

    varRadius.init();

    // mirror the first octant to the second
    for (int i = 0; i < 22; i++)
    {
        points[44-i].x = -points[i].y - 1;
        points[44-i].y = -points[i].x - 1;
    }

    // mirror first quarter to the fourth
    for (int i = 0; i < 45; i++)
    {
        points[89-i].x = points[i].x;
        points[89-i].y = -points[i].y - 1;
    }

    for (int i = 0; i < 90; i++)
    {
        points[179-i].x = -points[i].x - 1;
        points[179-i].y = points[i].y;
    }

    // offset points
    for (Point &point : points)
    {
        point.x += 32;
        point.y += 32;
    }


}

#define UPDATES_PER_SECOND (NUM_POINTS/60)

void loop()
{
    static int last_second = -1;

    g_webServer.handleClient();
    g_timeInfo = NTP_getTime();

    static int activePixels = 0;
    static uint32_t nextUpdate = 0;
    static int updatesThisSecond = 0;
    const  uint32_t now = millis();

    if (last_second == g_timeInfo.tm_sec)
    {
        if (now >= nextUpdate && updatesThisSecond < UPDATES_PER_SECOND)
        {
            nextUpdate = now + 1000 / UPDATES_PER_SECOND;
            ++updatesThisSecond;
        }
        else
        {
            return;
        }
    }
    else
    {
        activePixels = g_timeInfo.tm_sec * UPDATES_PER_SECOND;
        last_second = g_timeInfo.tm_sec;
        updatesThisSecond = 0;
    }

    g_display.clearDisplay();

    // for (Frame *frame : frames)
    //     frame->show();

    g_display.setTextColor(g_colorPrimary);
    g_display.setTextSize(3);
    g_display.setCursor(15, 9);
    if (g_timeInfo.tm_hour < 10)
        g_display.print("0");
    g_display.print(g_timeInfo.tm_hour);
    g_display.setCursor(15, 33);
    if (g_timeInfo.tm_min < 10)
        g_display.print("0");
    g_display.print(g_timeInfo.tm_min);

    int radius = varRadius._value.toInt();

    uint16_t colorA = g_timeInfo.tm_min & 1 ? g_colorPrimary : 0;
    uint16_t colorB = g_timeInfo.tm_min & 1 ? 0 : g_colorPrimary;
    
    for (int i = 0; i < NUM_POINTS; i++)
        g_display.drawPixel(points[i].x, points[i].y, colorA);

    for (int i = 0; i < activePixels; i++)
        g_display.drawPixel(points[i].x, points[i].y, colorB);

    ++activePixels %= NUM_POINTS + 1;

    g_display.showBuffer();
}
