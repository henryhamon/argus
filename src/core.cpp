#include "core.h"

// State variables (Persistent)
unsigned long lastCleanTime = 0;
int daysSinceLastClean = 7; // Start at 7 for testing

// Helper: Timestamp generator
String getTimestamp() {
    unsigned long now = millis();
    unsigned long h = (now / 3600000) % 24;
    unsigned long m = (now / 60000) % 60;
    unsigned long s = (now / 1000) % 60;
    char buf[16];
    sprintf(buf, "[%02lu:%02lu:%02lu]", h, m, s);
    return String(buf);
}

void logSystem(String message) {
    Serial.print(getTimestamp());
    Serial.print(" ");
    Serial.println(message);
}

SystemMode determineOperationMode(float lux) {
    return (lux >= MIN_LUX_DAY_MODE) ? MODE_DAY : MODE_NIGHT;
}

float calculateEfficiency(float lux) {
    if (lux == 0) return 0;
    
    // Simulation logic (replace with real INA219 reading later)
    float expectedPower = (lux / 1000.0) * 5.0; 
    
    // Simulating low efficiency (72%) to test alerts
    float actualPower = expectedPower * 0.72; 

    return (actualPower / expectedPower) * 100.0;
}

void evaluateSystemState(SystemStatus status) {
    bool triggerCleaning = false;
    String reason = "";

    // Rule 1: Environmental (High Dust + Low Hum + Sun + Schedule)
    bool condDust = (status.dust > DUST_THRESHOLD);
    bool condHum = (status.humidity < HUMIDITY_MIN_TRIGGER);
    bool condLux = (status.lux > MIN_LUX_FOR_CLEANING);
    bool condTime = (daysSinceLastClean >= DAYS_BETWEEN_CLEAN);

    if (condDust && condHum && condLux && condTime) {
        triggerCleaning = true;
        reason = "Environment (Dust+Dry+Time)";
    }

    // Rule 2: Performance (Low Efficiency + High Sun)
    bool condEff = (status.efficiency < EFFICIENCY_MIN);
    bool condLuxHigh = (status.lux > 10000);

    if (condEff && condLuxHigh) {
        triggerCleaning = true;
        if (reason.length() > 0) reason += " & ";
        reason += "Performance (Low Efficiency)";
    }

    // Action execution
    if (triggerCleaning) {
        logSystem("⚠️ ALERT: CLEANING TRIGGERED");
        logSystem("   Reason: " + reason);
        logSystem("   Action: Capturing Evidence...");
        
        // Reset counters
        lastCleanTime = millis();
        daysSinceLastClean = 0;
    } else {
        logSystem("Status: Normal Operation");
    }
}