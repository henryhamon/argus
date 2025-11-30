/*
 * ============================================================================
 * ArgoS - secrets_template.h
 * ============================================================================
 * Template for secrets configuration
 * 
 * SETUP INSTRUCTIONS:
 * 1. Copy this file and rename to: secrets.h
 * 2. Fill in your actual credentials
 * 3. NEVER commit secrets.h to git!
 * ============================================================================
*/

#ifndef SECRETS_H
#define SECRETS_H

// ============================================================================
// WiFi CREDENTIALS
// ============================================================================

#define SECRET_SSID "YOUR_WIFI_NETWORK_NAME"
#define SECRET_WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ============================================================================
// MQTT CREDENTIALS
// ============================================================================

#define SECRET_MQTT_SERVER "broker.hivemq.com"
#define SECRET_MQTT_PORT 1883
#define SECRET_MQTT_USER "your_mqtt_username"
#define SECRET_MQTT_PASSWORD "your_mqtt_password"

#define SECRET_MQTT_CLIENT_ID "ArgoS_Station_001"


 #endif // ARDUINO_SECRETS_H