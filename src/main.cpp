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
* Version: 1.0.4
* Date: 2025-11-22
* ============================================================================
*/


#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <BH1750.h>
#include "esp_camera.h"
#include "config.h"

// Objetos
DHT dht(DHT_PIN, DHT_TYPE);
BH1750 lightMeter(BH1750_ADDR);

// Função auxiliar para inicializar a câmera
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
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 12;          // 0-63 (menor é melhor qualidade)
    config.fb_count = 1;
    
    // PSRAM Check
    if(psramFound()){
        config.fb_count = 2;
        Serial.println("   -> PSRAM detectada (Buffer Duplo ativado)");
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("❌ Erro Camera: 0x%x\n", err);
        return false;
    }
    return true;
}

float lerPoeira() {
    digitalWrite(DUST_LED_PIN, LOW); 
    delayMicroseconds(280);          
    int adc = analogRead(DUST_VO_PIN); 
    delayMicroseconds(40);
    digitalWrite(DUST_LED_PIN, HIGH); 
    delayMicroseconds(9680);

    float voltagem = adc * (3.3 / 4095.0);
    // Ajuste simples para evitar negativos no teste
    if (voltagem < 0.1) voltagem = 0.1; 
    
    float densidade = (0.17 * voltagem - 0.1) * 1000 * DUST_CALIB;
    if (densidade < 0) densidade = 0;
    return densidade;
}

void setup() {
    delay(3000); 
    Serial.begin(115200);
    Serial.println("\n\n=== FASE 5: SISTEMA COMPLETO (ArguS) ===");

    // 1. Sensores
    dht.begin();
    Wire.setTimeOut(1000);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    
    pinMode(DUST_LED_PIN, OUTPUT);
    digitalWrite(DUST_LED_PIN, HIGH);
    analogReadResolution(12);
    
    // 2. Câmera
    Serial.println("• Inicializando Câmera...");
    if (initCamera()) {
        Serial.println("✅ Câmera OV2640 Iniciada!");
    }
}

void loop() {
    Serial.println("\n--- Status do Sistema ---");

    // Teste Câmera: Tira uma foto e descarta (só para testar o barramento)
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("📷 Câmera: FALHA na captura");
    } else {
        Serial.printf("📷 Câmera: Captura OK (%u bytes)\n", fb->len);
        esp_camera_fb_return(fb); // Importante: Devolver memória
    }

    // Sensores
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    float l = lightMeter.readLightLevel();
    float p = lerPoeira();

    Serial.printf("🌡️ %.1f°C | 💧 %.1f%% | 💡 %.1f lx | 🌫️ %.0f ug/m3\n", t, h, l, p);

    delay(3000);
}