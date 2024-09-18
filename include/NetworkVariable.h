#pragma once

#include <Arduino.h>

#include "FWebServer.h"

struct NetworkVariable
{
    NetworkVariable(const char *name) : _name(name) {}
    NetworkVariable(const char *name, std::function<void(String const&)> callback) : _name(name), _callback(callback) {}

    void init()
    {
        g_webServer.on("/" + _name, [this] {
            if (g_webServer.hasArg("val")) {
                _value = g_webServer.arg("val");

                if (_callback)
                    _callback(_value);
                    
                g_webServer.send(200, "text/plain", "Value set\n");
            }
            else {
                g_webServer.send(400, "text/plain", "Invalid request, please provide a \"val\" argument.\n");
            }
        });
    }

    String _name;
    String _value;
    std::function<void(String const&)> _callback;
};