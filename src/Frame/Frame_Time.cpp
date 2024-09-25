#include <Frame/Frame_Time.h>

// #include <PxMatrix.h>

void Frame_Time::show()
{
    Frame::show();

    const int FONT_SIZE = 2;

    String text;
    text.reserve(5);

    if (g_timeInfo.tm_hour < 10)
        text += "0";
    text += g_timeInfo.tm_hour;
    text += (g_timeInfo.tm_sec & 1) ? ':' : ' ';
    if (g_timeInfo.tm_min < 10)
        text += "0";
    text += g_timeInfo.tm_min;

    printCentered(text, FONT_SIZE);

    const int x = _pos.x + 1; // dimensioned from inside the frame
    const int y = _pos.y + 1;
    const int l = g_timeInfo.tm_sec + 1;

    uint16_t lineColor = (g_timeInfo.tm_min & 1) ? 0x0000 : g_colorPrimary;
    uint16_t bgColor = (g_timeInfo.tm_min & 1) ? g_colorPrimary : 0x0000;

    g_display.drawFastHLine(x, y, 60, bgColor);
    g_display.drawFastHLine(x, y, l, lineColor);

    g_display.drawFastHLine(x, y + _size.h - 3, 60, lineColor);
    g_display.drawFastHLine(x, y + _size.h - 3, l, bgColor);
}