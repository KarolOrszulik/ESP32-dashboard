#include <WiFiClient.h>
#include <PubSubClient.h>

WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);


void MQTT_init(const char* mqtt_server)
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback([](char* topic, byte* payload, unsigned int length) {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();
    });
}

void ensureMQTTConnection()
{
    if (!mqttClient.connected())
    {
        Serial.print("MQTT not connected, reconnecting...");
        if (mqttClient.connect("esp32dash"))
        {
            Serial.println("MQTT connected");
            mqttClient.subscribe("esp32/output");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
        }
    }
}