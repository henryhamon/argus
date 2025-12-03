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
#define PROJECT_VERSION "1.0-modular"
#define HARDWARE_MODEL "Freenove ESP32-S3 WROOM CAM"  // ← ADICIONADO

// Use secret or fallback to default
#ifdef SECRET_MQTT_CLIENT_ID
    #define DEVICE_ID SECRET_MQTT_CLIENT_ID
#else
    #define DEVICE_ID "ArgoS_Station_001"
#endif

// ============================================================================
// HARDWARE PINOUT
// ============================================================================

// --- Camera (Freenove ESP32-S3 WROOM) ---
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    15
#define SIOD_GPIO_NUM    4
#define SIOC_GPIO_NUM    5
#define Y9_GPIO_NUM      16
#define Y8_GPIO_NUM      17
#define Y7_GPIO_NUM      18
#define Y6_GPIO_NUM      12
#define Y5_GPIO_NUM      10
#define Y4_GPIO_NUM      8
#define Y3_GPIO_NUM      9
#define Y2_GPIO_NUM      11
#define VSYNC_GPIO_NUM   6 
#define HREF_GPIO_NUM    7
#define PCLK_GPIO_NUM    13

// --- Sensors ---
#define DHT_PIN          47      
#define DHT_TYPE         DHT22   

#define BH1750_ADDR      0x23
#define I2C_SDA_PIN      2
#define I2C_SCL_PIN      1

#define DUST_LED_PIN     21      
#define DUST_VO_PIN      3      

// ============================================================================
// OPERATIONAL THRESHOLDS (Business Logic)
// ============================================================================

// Light
#define MIN_LUX_DAY_MODE      1000      // Below this = Night Mode
#define MIN_LUX_FOR_CLEANING  5000      // Min lux to justify cleaning
#define MAX_LUX_REFERENCE     100000    // Full sun reference

// Environment
#define DUST_THRESHOLD        150.0     // ug/m3 (High risk)
#define HUMIDITY_MIN_TRIGGER  60.0      // % (Low humidity increases dust sticking)
#define DUST_CALIB            1.1       // Calibration factor
#define DUST_SAMPLES          5         // Moving average window size

// Performance
#define EFFICIENCY_MIN        75.0      // % 
#define PANEL_RATED_POWER     550.0     // Watts

// Timing (Milliseconds)
// Production settings: Day=5min (300000), Night=60min (3600000)
#define INTERVAL_DAY          10000     // 10 sec (For Testing)
#define INTERVAL_NIGHT        30000     // 30 sec (For Testing)
#define DAYS_BETWEEN_CLEAN    7

// ============================================================================
// MQTT CONFIGURATION
// ============================================================================

// Base Topic Structure: argus/{device_id}/{category}/{metric}
#define TOPIC_PREFIX "argus/"

// Sub-topics (suffix)
#define TOPIC_DUST        "sensor/dust_density"
#define TOPIC_TEMP        "sensor/temperature"
#define TOPIC_HUM         "sensor/humidity"
#define TOPIC_LUX         "sensor/light_level"
#define TOPIC_ALERT       "alert/clean_needed"
#define TOPIC_MODE        "status/operation_mode"

// Camera Topics
#define TOPIC_CAM_CTRL    "camera/control"     // JSON metadata (start/end)
#define TOPIC_CAM_DATA    "camera/image_chunk" // Binary data
#define TOPIC_CAM_ACK     "camera/ack"         // Command from Dashboard to ESP

// Image Config
#define IMG_CHUNK_SIZE    2048  // 2KB per chunk (safe for HiveMQ free tier)

#endif

