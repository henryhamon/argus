#include "mqtt_driver.h"
#include <time.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);
char topicBuffer[128];

const char* getTopic(const char* suffix) {
    snprintf(topicBuffer, sizeof(topicBuffer), "%s%s/%s", TOPIC_PREFIX, SECRET_MQTT_CLIENT_ID, suffix);
    return topicBuffer;
}

void callback(char* topic, byte* payload, unsigned int length) {
    // Futuro: Implementar recepção de ACK aqui se necessário
    // Por enquanto, apenas logamos
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void syncTime() {
    Serial.print("⏳ NTP Sync...");
    configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 
    
    time_t now = time(nullptr);
    int retry = 0;
    while (now < 100000 && retry < 20) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
        retry++;
    }
    
    if (now > 100000) {
        Serial.println(" OK!");
    } else {
        Serial.println(" Failed (Time is wrong, SSL might fail)");
    }
}

void initMQTT() {
    syncTime();

    espClient.setInsecure();
    espClient.setTimeout(15);

    Serial.print("🔍 DNS check: ");
    IPAddress ip;
    if(WiFi.hostByName(SECRET_MQTT_SERVER, ip)) {
        Serial.print("OK (");
        Serial.print(ip);
        Serial.println(")");
    } else {
        Serial.println("❌ DNS FAIL! Check secrets.h");
    }

    client.setServer(SECRET_MQTT_SERVER, SECRET_MQTT_PORT);
    client.setCallback(callback);
    client.setBufferSize(MQTT_MAX_PACKET_SIZE); 
}

void reconnect() {
    if (!client.connected()) {
        Serial.print("📡 Connecting to HiveMQ...");
        if (client.connect(SECRET_MQTT_CLIENT_ID, SECRET_MQTT_USER, SECRET_MQTT_PASSWORD)) {
            Serial.println(" Connected!");
            client.subscribe(getTopic(TOPIC_CAM_ACK));
            client.publish(getTopic(TOPIC_MODE), "BOOT_ONLINE");
        } else {
            Serial.print(" failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in next loop");
        }
    }
}

void loopMQTT() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
    }
}

bool publishTelemetry(SystemStatus status) {
    if (!client.connected()) return false;

    client.publish(getTopic(TOPIC_TEMP), String(status.temp, 1).c_str());
    client.publish(getTopic(TOPIC_HUM), String(status.humidity, 1).c_str());
    client.publish(getTopic(TOPIC_LUX), String(status.lux, 0).c_str());
    client.publish(getTopic(TOPIC_DUST), String(status.dust, 0).c_str());
    
    return true;
}

bool publishState(String mode) {
    if (!client.connected()) return false;
    return client.publish(getTopic(TOPIC_MODE), mode.c_str());
}

bool publishAlert(bool cleanNeeded, String reason) {
    if (!client.connected()) return false;
    client.publish(getTopic(TOPIC_ALERT), cleanNeeded ? "true" : "false");
    return true;
}

bool publishImage(const uint8_t* imageBuffer, size_t length) {
    if (!client.connected()) return false;

    Serial.printf("📸 Starting Image Upload (%u bytes)...\n", length);

    // 1. Send START Metadata (JSON)
    StaticJsonDocument<200> doc;
    doc["status"] = "start";
    doc["size"] = length;
    doc["device"] = SECRET_MQTT_CLIENT_ID;
    char jsonBuffer[200];
    serializeJson(doc, jsonBuffer);
    client.publish(getTopic(TOPIC_CAM_CTRL), jsonBuffer);

    // 2. Stream Binary Chunks
    size_t offset = 0;
    size_t chunkIndex = 0;
    
    while (offset < length) {
        size_t chunkSize = length - offset;
        if (chunkSize > IMG_CHUNK_SIZE) chunkSize = IMG_CHUNK_SIZE;

        client.publish(getTopic(TOPIC_CAM_DATA), &imageBuffer[offset], chunkSize);
        offset += chunkSize;
        chunkIndex++;
        
        delay(5); 
        client.loop(); 
    }

    // 3. Send END Metadata
    doc.clear();
    doc["status"] = "end";
    doc["chunks"] = chunkIndex;
    
    serializeJson(doc, jsonBuffer);
    client.publish(getTopic(TOPIC_CAM_CTRL), jsonBuffer);

    Serial.println("📸 Image Upload Complete.");
    return true;
}