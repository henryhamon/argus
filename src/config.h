/*
* ============================================================================
* ArgoS - config.h
* ============================================================================
* All constants, pin definitions, and system parameters
* ============================================================================
*/

#ifndef ARGOS_CONFIG_H
#define ARGOS_CONFIG_H

// ============================================================================
// IMPORT SECRETS (credentials not in version control)
// ============================================================================

#include "secrets.h"

// ============================================================================
// PROJECT IDENTIFICATION
// ============================================================================

#define PROJECT_NAME "ArgoS"
#define PROJECT_VERSION "2.1-modular"

// Use secret or fallback to default
#ifdef SECRET_MQTT_CLIENT_ID
#define DEVICE_ID SECRET_MQTT_CLIENT_ID
#else
#define DEVICE_ID "ArgoS_Station_001"
#endif

// ============================================================================
// WiFi SETTINGS (using secrets)
// ============================================================================

#define WIFI_SSID SECRET_SSID
#define WIFI_PASSWORD SECRET_WIFI_PASSWORD
#define WIFI_TIMEOUT_MS 20000  // 20 seconds

// ============================================================================
// MQTT SETTINGS (using secrets)
// ============================================================================

#define MQTT_SERVER SECRET_MQTT_SERVER
#define MQTT_PORT SECRET_MQTT_PORT
#define MQTT_USER SECRET_MQTT_USER
#define MQTT_PASSWORD SECRET_MQTT_PASSWORD
#define MQTT_CLIENT_ID DEVICE_ID
#define MQTT_BUFFER_SIZE 16384  // 16KB for images

// MQTT Topics
#define TOPIC_BASE "argos/"

// Sensors
#define TOPIC_DUST          TOPIC_BASE "sensor/dust_density"
#define TOPIC_TEMP          TOPIC_BASE "sensor/temperature"
#define TOPIC_HUMIDITY      TOPIC_BASE "sensor/humidity"
#define TOPIC_LIGHT         TOPIC_BASE "sensor/light_level"
#define TOPIC_EFFICIENCY    TOPIC_BASE "sensor/panel_efficiency"

// Alerts
#define TOPIC_ALERT         TOPIC_BASE "alert/clean_needed"
#define TOPIC_ALERT_REASON  TOPIC_BASE "alert/reason"

// Status
#define TOPIC_MODE          TOPIC_BASE "status/operation_mode"
#define TOPIC_STATUS        TOPIC_BASE "status/system"

// Camera
#define TOPIC_CAMERA_CONTROL TOPIC_BASE "camera/control"
#define TOPIC_CAMERA_IMAGE   TOPIC_BASE "camera/image_chunk"
#define TOPIC_CAMERA_STATUS  TOPIC_BASE "camera/status"

// Control (subscribe)
#define TOPIC_CONTROL       TOPIC_BASE "control/#"

// ============================================================================
// PINS - GP2Y1010AU0F (Dust Sensor)
// ============================================================================

#define DUST_LED_PIN 25         // GPIO to control IR LED
#define DUST_SENSOR_PIN 34      // GPIO ADC to read Vo (ADC1_CH6)

// Calibration (220Ω resistor instead of 150Ω)
#define DUST_CALIB_FACTOR 0.92

// ============================================================================
// PINS - DHT22 (Temperature and Humidity)
// ============================================================================

#define DHT_PIN 4
#define DHT_TYPE DHT22

// ============================================================================
// PINS - BH1750 (Light Intensity Sensor)
// ============================================================================

#define BH1750_I2C_ADDR 0x23
#define BH1750_SDA_PIN 21
#define BH1750_SCL_PIN 22

// ============================================================================
// PINS - ESP32-S3 CAMERA
// ============================================================================

// IMPORTANT: Adjust according to your specific module
// If SECRET_CAMERA_PWDN_PIN is defined in secrets.h, use it
#ifdef SECRET_CAMERA_PWDN_PIN
#define CAM_PIN_PWDN SECRET_CAMERA_PWDN_PIN
#else
#define CAM_PIN_PWDN -1      // -1 if not available
#endif

#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    0
#define CAM_PIN_SIOD    26      // SDA
#define CAM_PIN_SIOC    27      // SCL

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0      5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

// ============================================================================
// PARAMETERS - LIGHT INTENSITY
// ============================================================================

#define MIN_LUX_OPERATION 1000      // Day/night threshold
#define MIN_LUX_FOR_CLEANING 5000   // Minimum light to trigger cleaning
#define MAX_LUX_REFERENCE 100000    // Full sunlight reference

// ============================================================================
// PARAMETERS - AIR QUALITY / DUST
// ============================================================================

#define DUST_THRESHOLD 150.0        // µg/m³ - Critical threshold for alert
#define DUST_HEALTHY_MAX 50.0       // µg/m³ - Healthy air
#define DUST_MODERATE_MAX 150.0     // µg/m³ - Moderate
#define DUST_MAX_VALID 600.0        // µg/m³ - Maximum valid sensor reading

#define DUST_SAMPLES 5              // Number of samples for moving average

// ============================================================================
// PARAMETERS - WEATHER
// ============================================================================

#define HUMIDITY_THRESHOLD 60.0     // % - Low probability of rain
#define TEMP_MIN 0.0                // °C - Minimum operating temperature
#define TEMP_MAX 60.0               // °C - Maximum operating temperature

// ============================================================================
// PARAMETERS - PANEL PERFORMANCE
// ============================================================================

#define EFFICIENCY_THRESHOLD 75.0   // % - Minimum acceptable efficiency
#define PANEL_RATED_POWER 550.0     // W - Panel rated power

// ============================================================================
// PARAMETERS - TIMING
// ============================================================================

#define READING_INTERVAL_DAY 300000     // 5 minutes (ms)
#define READING_INTERVAL_NIGHT 3600000  // 60 minutes (ms)
#define MIN_DAYS_BETWEEN_CLEAN 7        // Minimum days between cleanings

// ============================================================================
// PARAMETERS - ESP32-S3 ADC
// ============================================================================

#define ADC_MAX 4095                // 12-bit resolution
#define ADC_VREF 3.3                // Reference voltage (V)

// ============================================================================
// PARAMETERS - CAMERA
// ============================================================================

// Reduced clock for power saving and heat reduction
#define CAMERA_XCLK_FREQ 10000000   // 10MHz (instead of 20MHz)
#define CAMERA_WARMUP_TIME 2000     // 2 seconds stabilization
#define CAMERA_FRAME_SIZE FRAMESIZE_SVGA  // 800x600
#define CAMERA_JPEG_QUALITY 12      // 0-63 (lower = better quality)

// Image fragmentation via MQTT
#define CAMERA_CHUNK_SIZE 8192      // 8KB per chunk

// ============================================================================
// PARAMETERS - DEBUGGING
// ============================================================================

#define DEBUG_MODE true             // Enable debug messages
#define SERIAL_BAUD_RATE 115200

// Macros for conditional debugging
#if DEBUG_MODE
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(...)
#endif

// ============================================================================
// FORMULAS AND CONVERSIONS
// ============================================================================

// ADC → Voltage conversion
#define ADC_TO_VOLTAGE(adc) ((float)(adc) * ADC_VREF / ADC_MAX)

// Voltage → Dust density (µg/m³) conversion
// Formula: D[mg/m³] = 0.170 × Vo - 0.1
// Multiplied by 1000 for µg/m³ and calibration factor
#define VOLTAGE_TO_DUST(v) ((0.170 * (v) - 0.1) * 1000.0 * DUST_CALIB_FACTOR)

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Structure to store sensor readings
struct SensorReadings {
    float dust;           // µg/m³
    float temperature;    // °C
    float humidity;       // %
    float light;          // lux
    float efficiency;     // % (if available)
    unsigned long timestamp;  // millis()
};

// Structure for system state
struct SystemState {
    bool dayMode;
    bool cameraInitialized;
    bool wifiConnected;
    bool mqttConnected;
    unsigned long lastReadingTime;
    unsigned long readingInterval;
};

// ============================================================================
// ENUMERATIONS
// ============================================================================

enum OperationMode {
    MODE_INIT,
    MODE_DAY,
    MODE_NIGHT,
    MODE_ERROR
};

enum CameraState {
    CAM_STANDBY,
    CAM_INITIALIZING,
    CAM_READY,
    CAM_CAPTURING,
    CAM_ERROR
};

enum AirQuality {
    AIR_HEALTHY,      // < 50 µg/m³
    AIR_MODERATE,     // 50-150 µg/m³
    AIR_UNHEALTHY     // > 150 µg/m³
};

#endif // ARGOS_CONFIG_H
