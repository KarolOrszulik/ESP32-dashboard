#include <BDisplay.h>
#include <BWiFi.h>
#include <BNTP.h>
#include <BWebServer.h>
#include <BMQTT.h>

void setup()
{
    Serial.begin(115200);
    delay(500);

    #include "wifi_credentials.h"
    const char* WIFI_HOSTNAME = "esp32dash";
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


void loop()
{
    webServer.handleClient();
    ensureMQTTConnection();
    mqttClient.loop();


    static int last_second = -1;

    struct tm timeinfo = NTP_getTime();

    if (timeinfo.tm_sec == last_second)
        return;

    last_second = timeinfo.tm_sec;
    

    display.clearDisplay();
    display.setTextColor(color);
    display.setTextSize(2);
    display.setCursor(3, 25);

    if (timeinfo.tm_hour < 10)
        display.print("0");
    display.print(timeinfo.tm_hour);
    display.print((timeinfo.tm_sec & 1) ? ':' : ' ');
    if (timeinfo.tm_min < 10)
        display.print("0");
    display.print(timeinfo.tm_min);
    


    display.drawFastHLine(0, 0,  64, 0xFFFF);
    display.drawFastHLine(0, 1,  64, 0xFFFF);
    display.drawFastHLine(0, 62, 64, 0xFFFF);
    display.drawFastHLine(0, 63, 64, 0xFFFF);

    const int h = timeinfo.tm_sec + 1;
    const uint16_t line_color = timeinfo.tm_min % 2 ? color : 0x0000;
    const uint16_t bg_color =   timeinfo.tm_min % 2 ? 0x0000 : color;
    display.drawFastVLine(0, 2, 60, bg_color);
    display.drawFastVLine(63, 2, 60, bg_color);
    display.drawFastVLine(0, 62-h, h, line_color);
    display.drawFastVLine(63, 2, h, line_color);

    
    display.showBuffer();

    delay(100);
}
