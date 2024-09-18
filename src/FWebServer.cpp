#include "FWebServer.h"
#include "NetworkVariable.h"
#include "FDisplay.h"

WebServer g_webServer(80);

NetworkVariable varBrightness{"brightness", [] (String const &val) {
    int value = val.toInt();
    if (value < 0 || value > 255)
    {
        g_webServer.send(400, "text/plain", "Brightness must be between 0 and 255.\n");
        return;
    }
    g_display.setBrightness(val.toInt());
}};

NetworkVariable varTime{"time", [] (String const &val) {
    int value = val.toInt();
    if (value < 1 || value > 100)
    {
        g_webServer.send(400, "text/plain", "Time must be between 1 and 100.\n");
        return;
    }
    g_ledDuration = val.toInt();
}};

NetworkVariable varWhite{"white", [] (String const &val) {
    int value = val.toInt();
    if (value < 0 || value > 255)
    {
        g_webServer.send(400, "text/plain", "White must be between 0 and 255.\n");
        return;
    }
    
    if (g_webServer.hasArg("type") && g_webServer.arg("type") == "2")
        g_colorSecondary = g_display.color565(value, value, value);
    else
        g_colorPrimary = g_display.color565(value, value, value);
}};

NetworkVariable varHue{"hue", [] (String const &val) {
    int value = val.toInt();
    if (value < 0 || value > 360)
    {
        g_webServer.send(400, "text/plain", "Hue must be between 0 and 360.\n");
        return;
    }

    // rgb conversion for hue = [0, 360]
    const int r = (value < 60) ? 255 : (value < 120) ? 255 - (value - 60) * 255 / 60 : (value < 240) ? 0 : (value < 300) ? (value - 240) * 255 / 60 : 255;
    const int g = (value < 60) ? value * 255 / 60 : (value < 180) ? 255 : (value < 240) ? 255 - (value - 180) * 255 / 60 : 0;
    const int b = (value < 120) ? 0 : (value < 180) ? (value - 120) * 255 / 60 : (value < 300) ? 255 : 255 - (value - 300) * 255 / 60;

    if (g_webServer.hasArg("type") && g_webServer.arg("type") == "2")
        g_colorSecondary = g_display.color565(r, g, b);
    else
        g_colorPrimary = g_display.color565(r, g, b);
}};


void WebServer_setCallbacks()
{
    g_webServer.on("/", HTTP_GET, [] {
        g_webServer.send(200, "text/html", "<h1>ESP32 Dash</h1>");
    });

    g_webServer.on("/", [] { g_webServer.send(200, "text/plain", "Hello, world!"); });

    g_webServer.on("/reset", [] {
        g_webServer.send(200, "text/plain", "Resetting...\n");
        ESP.restart();
    });

    g_webServer.on("/rgb", [] {
        if (g_webServer.hasArg("r") && g_webServer.hasArg("g") && g_webServer.hasArg("b"))
        {
            uint16_t color = g_display.color565(g_webServer.arg("r").toInt(), g_webServer.arg("g").toInt(), g_webServer.arg("b").toInt());

            if (g_webServer.hasArg("type") && g_webServer.arg("type").toInt() == 2)
                g_colorSecondary = color;
            else
                g_colorPrimary = color;

            g_webServer.send(200, "text/plain", "Color set\n");
        }
        else
        {
            g_webServer.send(400, "text/plain", "Invalid request\n");
        }
    });

    varBrightness.init();
    varTime.init();
    varWhite.init();
    varHue.init();
}