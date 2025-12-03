/*
* ============================================================================
* ArguS: Automated Remote Guardian Unit for Solar panels
* ============================================================================
* 
* Main file - Setup and Loop
* Modular version for incremental development
* 
* ============================================================================
* 
* Version: 1.0.5
* Date: 2025-11-22
* ============================================================================
*/

#include <Arduino.h>
#include "esp_camera.h"
#include "config.h"
#include "sensor_driver.h"
#include "core.h"
#include "mqtt_driver.h"

// Global State
SystemMode currentMode = MODE_BOOT;
unsigned long lastCheckTime = 0;

void setupWiFi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected.");
    } else {
        Serial.println("\nWiFi Failed (Will retry later).");
    }
}

// Camera Init (Kept here as it's a core system component)
bool initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    
    if(psramFound()) {
        config.fb_count = 2;
        logSystem("PSRAM Detected (Double Buffer)");
    }

    esp_err_t err = esp_camera_init(&config);
    return (err == ESP_OK);
}

void setup() {
    delay(3000);
    Serial.begin(115200);
    logSystem("=== ARGUS SYSTEM v1.0 STARTED ===");

    // 1. Hardware Init
    initSensors();
    
    if (initCamera()) {
        logSystem("✅ Camera Initialized");
    } else {
        logSystem("❌ Camera Failed");
    }

    setupWiFi();
    initMQTT();
    
    logSystem("System Ready. Waiting for cycle...");
}

void loop() {
    unsigned long now = millis();
    
    loopMQTT();

    // 1. Continuous Light Monitoring (Mode Switching)
    float currentLux = readLightLevel();
    SystemMode newMode = determineOperationMode(currentLux);

    if (newMode != currentMode) {
        currentMode = newMode;
        if (currentMode == MODE_DAY) logSystem("☀️ MODE CHANGE: DAY");
        else logSystem("🌙 MODE CHANGE: NIGHT");
    }

    // 2. Cycle Timing
    unsigned long interval = (currentMode == MODE_DAY) ? INTERVAL_DAY : INTERVAL_NIGHT;

    if (now - lastCheckTime > interval) {
        lastCheckTime = now;

        // Build Status Object
        SystemStatus status;
        status.lux = currentLux;
        status.mode = currentMode;

        if (currentMode == MODE_NIGHT) {
            logSystem("Night Monitor - Lux: " + String(status.lux));
        } 
        else {
            // Full Day Cycle
            status.temp = readTemperature();
            status.humidity = readHumidity();
            
            // Fill dust buffer (5 quick samples)
            for(int k=0; k<5; k++) { readDustSensorSmooth(); delay(20); }
            status.dust = readDustSensorSmooth();
            
            status.efficiency = calculateEfficiency(status.lux);

            // Log Data
            String logMsg = "Env: " + String(status.temp, 1) + "C | " + 
                            String(status.humidity, 1) + "% | " + 
                            String(status.lux, 0) + " lx | Dust: " + 
                            String(status.dust, 0) + " ug/m3";
            logSystem(logMsg);
            
            logSystem("Eff: " + String(status.efficiency, 1) + "%");

            if (publishTelemetry(status)) {
                Serial.println("📤 MQTT Sent");
            }

            // 3. DECISION CORE EXECUTION
            evaluateSystemState(status);
        }
    }
}