#include <App/App_BigClock.h>

#include <FDisplay.h>
#include <FNTP.h>

void App_BigClock::init()
{
    // mirror the first octant to the second
    for (int i = 0; i < 22; i++)
    {
        _points[44-i].x = -_points[i].y - 1;
        _points[44-i].y = -_points[i].x - 1;
    }

    // mirror first quarter to the fourth
    for (int i = 0; i < 45; i++)
    {
        _points[89-i].x = _points[i].x;
        _points[89-i].y = -_points[i].y - 1;
    }

    for (int i = 0; i < 90; i++)
    {
        _points[179-i].x = -_points[i].x - 1;
        _points[179-i].y = _points[i].y;
    }

    // offset points
    for (Point &point : _points)
    {
        point.x += 32;
        point.y += 32;
    }
}

void App_BigClock::select()
{
    // nothing   
}

void App_BigClock::show(int updatesThisSecond)
{
    const int activePixels = updatesThisSecond + g_timeInfo.tm_sec * getUpdatesPerSecond();

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


    uint16_t colorA = g_timeInfo.tm_min & 1 ? g_colorSecondary : 0;
    uint16_t colorB = g_timeInfo.tm_min & 1 ? 0 : g_colorSecondary;
    
    for (int i = 0; i < NUM_POINTS; i++)
        g_display.drawPixel(_points[i].x, _points[i].y, colorA);

    for (int i = 0; i < activePixels; i++)
        g_display.drawPixel(_points[i].x, _points[i].y, colorB);
}