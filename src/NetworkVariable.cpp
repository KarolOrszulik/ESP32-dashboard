#include "NetworkVariable.h"

NetworkVariable g_varXAxis{"xAxis"};
NetworkVariable g_varYAxis{"yAxis"};

void NetworkVariable::init()
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