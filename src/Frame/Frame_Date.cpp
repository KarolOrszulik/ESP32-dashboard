#include <Frame/Frame_Date.h>
#include <FNTP.h>

void Frame_Date::show()
{
    Frame::show();

    const int FONT_SIZE = 1;

    static const char *WEEKDAYS[] = {"Nie", "Pon", "Wto", "Sro", "Czw", "Pia", "Sob"};
    const String DAY_PADDING = g_timeInfo.tm_mday < 10 ? "0" : "";
    const String MONTH_PADDING = g_timeInfo.tm_mon + 1 < 10 ? "0" : "";
    String date =
        WEEKDAYS[g_timeInfo.tm_wday] +
        String(" ") + DAY_PADDING   + String(g_timeInfo.tm_mday) +
        "."         + MONTH_PADDING + String(g_timeInfo.tm_mon + 1);

    printCentered(date, FONT_SIZE);

    // hack for Polish diacritics
    if (g_timeInfo.tm_wday == 3) // środa
        g_display.drawFastVLine(_pos.x + 6, _pos.y + 2, 2, g_colorPrimary);
    else if (g_timeInfo.tm_wday == 5) // piątek
        g_display.drawFastVLine(_pos.x + 20, _pos.y + 12, 2, g_colorPrimary);
}