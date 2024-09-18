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
}

void loop()
{
    static int last_second = -1;

    g_webServer.handleClient();
    g_timeInfo = NTP_getTime();

    if (last_second == g_timeInfo.tm_sec)
        return;


    g_display.clearDisplay();

    for (Frame *frame : frames)
        frame->show();

    g_display.showBuffer();
}
