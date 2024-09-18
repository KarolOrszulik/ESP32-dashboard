#include "FWiFi.h"

void WiFi_connect(const char* ssid, const char* password, const char* hostname, uint32_t timeout)
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > timeout)
        {
            Serial.println("Failed to connect to WiFi");
            return;
        }
        Serial.print(".");
        delay(500);
    }

    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    
    
    if (MDNS.begin(hostname))
    {
        Serial.print("mDNS responder started: ");
        Serial.println(hostname);
    }
    else
    {
        Serial.println("Error setting up MDNS responder!");
    }
}