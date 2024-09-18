#include <WiFiClient.h>
#include <PubSubClient.h>

WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);

bool mqttConnected = false;

#define MAX_MQTT_TOPIC_LEN 64

struct MQTTMessage
{
    char topic[MAX_MQTT_TOPIC_LEN] = {0};
    char payload[MAX_MQTT_TOPIC_LEN] = {0};
};

#define MAX_MQTT_TOPICS 10
MQTTMessage mqttMessages[MAX_MQTT_TOPICS];

void MQTT_addSubscribtion(const char* topic)
{
    for (int i = 0; i < MAX_MQTT_TOPICS; i++)
    {
        if (strlen(mqttMessages[i].topic) == 0)
        {
            strcpy(mqttMessages[i].topic, topic);
            break;
        }
    }
}

char* MQTT_lastMessageFrom(const char* topic)
{
    for (int i = 0; i < MAX_MQTT_TOPICS; i++)
    {
        if (strcmp(mqttMessages[i].topic, topic) == 0)
        {
            return mqttMessages[i].payload;
        }
    }
    return nullptr;
}

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

        for (int i = 0; i < MAX_MQTT_TOPICS; i++)
        {
            if (strcmp(mqttMessages[i].topic, topic) == 0)
            {
                strncpy(mqttMessages[i].payload, (char*)payload, length);
                break;
            }
        }
    });
}

void ensureMQTTConnection()
{
    mqttConnected = mqttClient.connected();
    if (!mqttConnected)
    {
        Serial.print("MQTT not connected, reconnecting...");
        if (mqttClient.connect("esp32dash"))
        {
            Serial.println("MQTT connected");
            mqttClient.subscribe("esp32dash/debug");

            for (int i = 0; i < MAX_MQTT_TOPICS; i++)
            {
                if (strlen(mqttMessages[i].topic) > 0)
                {
                    mqttClient.subscribe(mqttMessages[i].topic);
                }
            }
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
        }
    }
}