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

// --- CÂMERA (Freenove S3 WROOM) ---
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

// --- DHT22 ---
#define DHT_PIN     47      
#define DHT_TYPE    DHT22   

// --- BH1750 --- LIGHT
#define BH1750_ADDR     0x23
#define I2C_SDA_PIN     2
#define I2C_SCL_PIN     1

// --- DUST (GP2Y1010AU0F) ---
#define DUST_LED_PIN    21      // Controle do LED Infravermelho
#define DUST_VO_PIN     3       // Leitura Analógica (ADC1)

// Calibração para 3.3V
// Ajuste este valor se a leitura for muito alta/baixa comparada à realidade
#define DUST_CALIB      1.1
#define DUST_OFFSET       0.1   // Tensão residual em Volts

// --- OPERATIONAL THRESHOLDS (REGRAS DE NEGÓCIO) ---

// Light thresholds
#define MIN_LUX_DAY_MODE      1000      // Abaixo disso = Modo Noturno
#define MIN_LUX_FOR_CLEANING  5000      // Mínimo para justificar limpeza
#define MAX_LUX_REFERENCE     100000    // Referência de sol pleno (para cálculo de eficiência)

// Environmental thresholds
#define DUST_THRESHOLD        150.0     // μg/m³ (Alto risco)
#define HUMIDITY_MIN_TRIGGER  60.0      // % (Abaixo disso + poeira = limpeza)
#define DUST_CALIB            1.1       // Fator de calibração
#define DUST_SAMPLES          5         // Quantidade de leituras para média móvel

// Performance thresholds
#define EFFICIENCY_MIN        75.0      // % 
#define PANEL_RATED_POWER     550.0     // Watts (Potência nominal do painel)

// Timing parameters (em milissegundos)
#define INTERVAL_DAY          10000     // 10 seg (TESTE) - Original: 5 min
#define INTERVAL_NIGHT        30000     // 30 seg (TESTE) - Original: 60 min
#define DAYS_BETWEEN_CLEAN    7         // Dias mínimos entre limpezas

#endif

