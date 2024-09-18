#include <BDisplay.h>
#include <BWiFi.h>
#include <BNTP.h>
#include <BWebServer.h>

struct tm timeinfo;

struct NetworkVariable
{
    NetworkVariable(const char *name) : _name(name) {}

    void init()
    {
        webServer.on("/" + _name, [this]
                     {
            if (webServer.hasArg("val")) {
                _value = webServer.arg("val");
                webServer.send(200, "text/plain", "Value set\n");
            } });
    }

    String _name;
    String _value;
};

NetworkVariable varTemperature{"temperature"};
NetworkVariable varHumidity{"humidity"};

void setup()
{
    Serial.begin(115200);
    delay(500);

#include "secret_wifi_credentials.h"
    WiFi_connect(WIFI_SSID, WIFI_PASSWORD, "esp32dash");

    WebServer_setCallbacks();
    varTemperature.init();
    varHumidity.init();
    webServer.begin();

    Display_init();
    Display_createRefreshTask();

#include "ntp_settings.h"
    NTP_configure(NTP_SERVER, GMT_OFFSET_S, DST_OFFSET_S);
}

class Frame
{
public:
    struct Size
    {
        union
        {
            int x, w;
        };
        union
        {
            int y, h;
        };
    };

    static constexpr Size FONT_SIZES[] = {{0, 0}, {6, 7}, {12, 14}, {18, 24}};

    Frame(Size pos, Size size) : _pos(pos), _size(size) {};

    virtual void show()
    {
        display.drawRect(_pos.x, _pos.y, _size.w, _size.h, colorSecondary);
    }

    Size margins(Size content) const
    {
        return {
            (_size.w - content.w) / 2,
            (_size.h - content.h) / 2};
    }

    Size margins(int charCount, int fontSize) const
    {
        return margins({charCount * FONT_SIZES[fontSize].w, FONT_SIZES[fontSize].h});
    }

    void printCentered(String const& text, int fontSize)
    {
        const Size MARGINS = margins(text.length(), fontSize);
        display.setTextColor(colorPrimary);
        display.setTextSize(fontSize);
        display.setCursor(_pos.x + MARGINS.x,
                          _pos.y + MARGINS.y);
        display.print(text);
    }

protected:
    Size _pos, _size;
};
constexpr Frame::Size Frame::FONT_SIZES[];


class Frame_Clock : public Frame
{
public:
    Frame_Clock(int height) // centered
        : Frame(
              Frame::Size{1, (display.height() - height) / 2},
              Frame::Size{62, height})
    {
    }

    void show() override
    {
        Frame::show();

        const int FONT_SIZE = 2;

        String text;
        text.reserve(5);

        if (timeinfo.tm_hour < 10)
            text += "0";
        text += timeinfo.tm_hour;
        text += (timeinfo.tm_sec & 1) ? ':' : ' ';
        if (timeinfo.tm_min < 10)
            text += "0";
        text += timeinfo.tm_min;

        printCentered(text, FONT_SIZE);


        const int x = _pos.x + 1; // dimensioned from inside the frame
        const int y = _pos.y + 1;
        const int l = timeinfo.tm_sec + 1;

        uint16_t lineColor = (timeinfo.tm_min & 1) ? 0x0000 : colorPrimary;
        uint16_t bgColor = (timeinfo.tm_min & 1) ? colorPrimary : 0x0000;

        display.drawFastHLine(x, y, 60, bgColor);
        display.drawFastHLine(x, y, l, lineColor);

        display.drawFastHLine(x, y + _size.h - 3, 60, lineColor);
        display.drawFastHLine(x, y + _size.h - 3, l, bgColor);
    }
};

class Frame_Date : public Frame
{
public:
    Frame_Date(Size pos, Size size)
        : Frame(
              Frame::Size{pos.x, pos.y},
              Frame::Size{size.w, size.h})
    {
    }

    void show() override
    {
        Frame::show();

        const int FONT_SIZE = 1;
        
        static const char* WEEKDAYS[] = {"Nie", "Pon", "Wto", "Sro", "Czw", "Pia", "Sob"};
        const String DAY_PADDING = timeinfo.tm_mday < 10 ? "0" : "";
        const String MONTH_PADDING = timeinfo.tm_mon + 1 < 10 ? "0" : "";
        String date =
            WEEKDAYS[timeinfo.tm_wday]
            + String(" ") + DAY_PADDING   + String(timeinfo.tm_mday)
            + "."         + MONTH_PADDING + String(timeinfo.tm_mon + 1);
        
        printCentered(date, FONT_SIZE);
    }
};

class Frame_NetworkVariable : public Frame
{
public:
    Frame_NetworkVariable(Size pos, Size size, NetworkVariable *nv)
        : Frame(pos, size), _nv(nv)
    {
    }

    void show() override
    {
        Frame::show();
        
        const int FONT_SIZE = 1;

        String text = _nv->_value;

        printCentered(text, FONT_SIZE);
    }

private:
    NetworkVariable *_nv;
};

Frame_NetworkVariable tempFrame({3, 1}, {28, 17}, &varTemperature);
Frame_Clock clockFrame(24);
Frame_Date  dateFrame({3, 46}, {58, 17});
Frame_NetworkVariable humidFrame({33, 1}, {28, 17}, &varHumidity);

Frame *frames[] = {&clockFrame, &tempFrame, &dateFrame, &humidFrame};

void loop()
{
    webServer.handleClient();
    timeinfo = NTP_getTime();

    display.clearDisplay();
    for (Frame *frame : frames)
    {
        frame->show();
    }
    display.showBuffer();

    delay(1000);
}
