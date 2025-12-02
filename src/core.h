#ifndef CORE_H
#define CORE_H

#include <Arduino.h>
#include "config.h"

// System Operation Modes
enum SystemMode { MODE_BOOT, MODE_DAY, MODE_NIGHT };

// Structure to hold complete system status
struct SystemStatus {
    float temp;
    float humidity;
    float lux;
    float dust;
    float efficiency;
    SystemMode mode;
};

// Calculates theoretical panel efficiency
float calculateEfficiency(float lux);

// Main logic engine: Evaluates inputs and triggers alerts if necessary
void evaluateSystemState(SystemStatus status);

// Helper to determine Day/Night based on Lux
SystemMode determineOperationMode(float lux);

// Helper to format logs
void logSystem(String message);

#endif