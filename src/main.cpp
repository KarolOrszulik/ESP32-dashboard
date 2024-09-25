#include <FDisplay.h>
#include <FWiFi.h>
#include <FNTP.h>
#include <FWebServer.h>

#include <App/App_Dashboard.h>
#include <App/App_BigClock.h>

App_Dashboard appDashboard;
App_BigClock  appBigClock;


App* apps[] = {&appDashboard, &appBigClock};
NetworkVariable varAppID{"appID"};

void setup()
{
    //
    // Serial setup
    //
    Serial.begin(115200);
    delay(500);

    //
    // WiFi setup
    //
#include "secret_wifi_credentials.h"
    WiFi_connect(WIFI_SSID, WIFI_PASSWORD, "esp32dash");

    //
    // WebServer setup
    //
    WebServer_setCallbacks();
    g_webServer.begin();

    //
    // NTP setup
    //
#include "ntp_settings.h"
    NTP_configure(NTP_SERVER, GMT_OFFSET_S, DST_OFFSET_S);

    //
    // Display setup
    //
    Display_init();

    // 
    // Apps setup
    //
    varAppID.init();
    for (App* app : apps)
        app->init();
}

void loop()
{
    g_webServer.handleClient();
    g_timeInfo = NTP_getTime();

    static App* currentApp = apps[0];

    static uint32_t nextUpdate = 0;
    static int updatesThisSecond = 0;
    static int updatesPerSecond = 0;

    const uint32_t NOW = millis();

    if (NTP_isNewSecond(g_timeInfo.tm_sec))
    {
        currentApp = apps[varAppID.getValueInt() % (sizeof(apps) / sizeof(apps[0]))];
        currentApp->select();
        updatesPerSecond = currentApp->getUpdatesPerSecond();

        updatesThisSecond = 0;
    }
    else
    {
        if (NOW < nextUpdate || updatesThisSecond >= updatesPerSecond)
            return;

        nextUpdate = NOW + 1000 / updatesPerSecond;
        ++updatesThisSecond;
    }


    g_display.clearDisplay();

    currentApp->show(updatesThisSecond);

    g_display.showBuffer();
}
