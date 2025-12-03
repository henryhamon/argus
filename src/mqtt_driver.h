#ifndef MQTT_DRIVER_H
#define MQTT_DRIVER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "core.h" // Para acessar a struct SystemStatus

void initMQTT();
void loopMQTT();
bool publishTelemetry(SystemStatus status);
bool publishState(String mode);
bool publishAlert(bool cleanNeeded, String reason);

/**
 * 1. Envia Metadata (Start, Size)
 * 2. Fatia o buffer da câmera em chunks
 * 3. Envia chunks binários
 * 4. Envia Metadata (End)
 */
bool publishImage(const uint8_t* imageBuffer, size_t length);

#endif