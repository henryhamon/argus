#include "sensor_driver.h"

// Private global objects
DHT dht(DHT_PIN, DHT_TYPE);
BH1750 lightMeter(BH1750_ADDR);

// Buffer for moving average
float dustReadings[DUST_SAMPLES];
int dustIndex = 0;

void initSensors() {
    dht.begin();
    
    Wire.setTimeOut(1000);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("✅ Sensors: BH1750 OK");
    } else {
        Serial.println("❌ Sensors: BH1750 Failed");
    }

    pinMode(DUST_LED_PIN, OUTPUT);
    digitalWrite(DUST_LED_PIN, HIGH);
    analogReadResolution(12);

    // Init dust buffer
    for(int i=0; i<DUST_SAMPLES; i++) dustReadings[i] = 0;
}

float readTemperature() {
    return dht.readTemperature();
}

float readHumidity() {
    return dht.readHumidity();
}

float readLightLevel() {
    float lux = lightMeter.readLightLevel();
    return (lux < 0) ? 0 : lux;
}

float readDustSensorSmooth() {
    // 1. Raw Read with precise timing
    digitalWrite(DUST_LED_PIN, LOW); 
    delayMicroseconds(280);          
    int adc = analogRead(DUST_VO_PIN); 
    delayMicroseconds(40);
    digitalWrite(DUST_LED_PIN, HIGH); 
    delayMicroseconds(9680);
    
    float voltage = adc * (3.3 / 4095.0);
    if (voltage < 0.1) voltage = 0.1;
    
    float rawDensity = (0.17 * voltage - 0.1) * 1000 * DUST_CALIB;
    if (rawDensity < 0) rawDensity = 0;

    // 2. Moving Average Logic
    dustReadings[dustIndex] = rawDensity;
    dustIndex = (dustIndex + 1) % DUST_SAMPLES;

    float sum = 0;
    for (int i = 0; i < DUST_SAMPLES; i++) sum += dustReadings[i];
    
    return sum / DUST_SAMPLES;
}