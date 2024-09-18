#include <BDisplay.h>
#include <BWiFi.h>
#include <BNTP.h>
#include <BWebServer.h>
#include <BMQTT.h>

void setup()
{
    Serial.begin(115200);
    delay(500);

#include "secret_wifi_credentials.h"
    const char *WIFI_HOSTNAME = "esp32dash";
    WiFi_connect(WIFI_SSID, WIFI_PASSWORD, WIFI_HOSTNAME);

    WebServer_setCallbacks();
    webServer.begin();

#include "mqtt_settings.h"
    MQTT_init(MQTT_BROKER);

    Display_init();
    Display_createRefreshTask();

#include "ntp_settings.h"
    NTP_configure(NTP_SERVER, GMT_OFFSET_S, DST_OFFSET_S);
}

class Frame // a frame on the display with data either from MQTT or NTP
{
public:
    virtual void show() = 0;

    Frame(uint16_t x, uint16_t y, uint16_t width, uint16_t height) : _x(x), _y(y), _width(width), _height(height) {}

protected:
    uint16_t _width, _height, _x, _y;
};

void Frame::show()
{
    {
        display.drawRect(_x, _y, _width, _height, colorSecondary);
    }
}

class Frame_Clock : public Frame
{
public:
    Frame_Clock(uint16_t y)
        : Frame(0, y, 64, 24)
    {
    }

    void show() override
    {
        Frame::show();
        display.drawRect(_x + 1, _y + 1, _width - 2, _height - 2, colorSecondary);

        struct tm timeinfo = NTP_getTime();

        display.setTextColor(colorPrimary);
        display.setTextSize(2);
        display.setCursor(_x + 3, _y + 5);

        if (timeinfo.tm_hour < 10)
            display.print("0");
        display.print(timeinfo.tm_hour);
        display.print((timeinfo.tm_sec & 1) ? ':' : ' ');
        if (timeinfo.tm_min < 10)
            display.print("0");
        display.print(timeinfo.tm_min);

        const int l = timeinfo.tm_sec + 1;
        uint16_t lineColor = (timeinfo.tm_min & 1) ? 0x0000 : colorPrimary;
        uint16_t bgColor = (timeinfo.tm_min & 1) ? colorPrimary : 0x0000;

        display.drawFastHLine(_x + 2, _y + 2, _width - 4, bgColor);
        display.drawFastHLine(_x + 2, _y + 2, l, lineColor);

        display.drawFastHLine(_x + 2, _y + _height - 3, _width - 4, lineColor);
        display.drawFastHLine(_x + 2, _y + _height - 3, l, bgColor);
    }
};

class Frame_MQTT : public Frame
{
public:
    Frame_MQTT(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *topic)
        : Frame(x, y, width, height)
    {
        MQTT_addSubscribtion(topic);
        strcpy(_topic, topic);
    }

    void show() override
    {
        Frame::show();
        display.setTextColor(colorPrimary);
        display.setTextSize(1);
        display.setCursor(_x + 2, _y + 2);
        display.print(MQTT_lastMessageFrom(_topic));
    }

private:
    char _topic[MAX_MQTT_TOPIC_LEN];
};

Frame_Clock clockFrame((64 - 24) / 2);
Frame_MQTT mqttFrame(0, 0, 32, 20, "esp32dash/temperature");

Frame *frames[] = {&clockFrame, &mqttFrame};

void loop()
{
    webServer.handleClient();
    ensureMQTTConnection();
    mqttClient.loop();

    display.clearDisplay();

    for (Frame* frame : frames)
    {
        frame->show();
    }

    display.showBuffer();

    delay(100);
}
