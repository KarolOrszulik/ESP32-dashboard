#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer webServer(80);

void WebServer_setCallbacks()
{
    webServer.on("/", [] { webServer.send(200, "text/plain", "Hello, world!"); });
    webServer.on("/favicon.ico", [] { webServer.send(404); });
    webServer.on("/reset", [] { ESP.restart(); });
    webServer.on("/rgb", [] {
        if (webServer.hasArg("r") && webServer.hasArg("g") && webServer.hasArg("b"))
        {
            colorPrimary = display.color565(webServer.arg("r").toInt(), webServer.arg("g").toInt(), webServer.arg("b").toInt());
            webServer.send(200, "text/plain", "Color set\n");
        }
        else
        {
            webServer.send(400, "text/plain", "Invalid request\n");
        }
    });
    webServer.on("/hue", [] {
        if (webServer.hasArg("hue"))
        {
            const int hue = webServer.arg("hue").toInt();

            if (hue < 0 || hue > 360)
            {
                webServer.send(400, "text/plain", "Hue must be between 0 and 360.\n");
                return;
            }

            // rgb conversion for hue = [0, 360]
            const int r = (hue < 60) ? 255 : (hue < 120) ? 255 - (hue - 60) * 255 / 60 : (hue < 240) ? 0 : (hue < 300) ? (hue - 240) * 255 / 60 : 255;
            const int g = (hue < 60) ? hue * 255 / 60 : (hue < 180) ? 255 : (hue < 240) ? 255 - (hue - 180) * 255 / 60 : 0;
            const int b = (hue < 120) ? 0 : (hue < 180) ? (hue - 120) * 255 / 60 : (hue < 300) ? 255 : 255 - (hue - 300) * 255 / 60;
            colorPrimary = display.color565(r, g, b);
            webServer.send(200, "text/plain", "Color set\n");
        }
        else
        {
            webServer.send(400, "text/plain", "Invalid request, please provide a \"hue\" argument in [0, 360].\n");
        }
    });
    webServer.on("/brightness", []{
        if (webServer.hasArg("brightness"))
        {
            const int brightness = webServer.arg("brightness").toInt();
            if (brightness < 0 || brightness > 255)
            {
                webServer.send(400, "text/plain", "Brightness must be between 0 and 255.\n");
                return;
            }
            display.setBrightness(brightness);
            webServer.send(200, "text/plain", "Brightness set\n");
        }
        else
        {
            webServer.send(400, "text/plain", "Invalid request, please provide a \"brightness\" argument in [0, 255].\n");
        }
    });
    webServer.on("/time", [] {
        if (webServer.hasArg("time"))
        {
            const int time = webServer.arg("time").toInt();
            if (time < 1 || time > 100)
            {
                webServer.send(400, "text/plain", "Time must be between 1 and 100.\n");
                return;
            }
            led_time = time;
            webServer.send(200, "text/plain", "Time set\n");
        }
        else
        {
            webServer.send(400, "text/plain", "Invalid request, please provide a \"time\" argument in [1, 100].\n");
        }
    });
}