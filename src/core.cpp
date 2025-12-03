#include "core.h"

// State variables (Persistent)
unsigned long lastCleanTime = 0;
int daysSinceLastClean = 7; // Start at 7 for testing

// Mocking function for testing
void setDaysSinceClean(int days) {
    daysSinceLastClean = days;
}

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

bool evaluateSystemState(SystemStatus status) {
    bool triggerCleaning = false;
    String reason = "";

    // --- RULE: Environmental ---
    bool condDust = (status.dust > DUST_THRESHOLD);
    bool condHum = (status.humidity < HUMIDITY_MIN_TRIGGER);
    bool condLux = (status.lux > MIN_LUX_FOR_CLEANING);
    bool condTime = (daysSinceLastClean >= DAYS_BETWEEN_CLEAN);

    // Visual Debug for Simulator
    if (condDust && condHum && condLux) {
        if (!condTime) {
            logSystem("⚠️ Critical conditions met, waiting for schedule (" + 
                    String(daysSinceLastClean) + "/" + String(DAYS_BETWEEN_CLEAN) + " days)");
        }
    }

    if (condDust && condHum && condLux && condTime) {
        triggerCleaning = true;
        reason = "Environment (Dust+Dry+Time)";
    }

    // --- ACTION ---
    if (triggerCleaning) {
        logSystem("⚠️ ALERT: CLEANING TRIGGERED");
        logSystem("   Reason: " + reason);
        
        lastCleanTime = millis();
        daysSinceLastClean = 0; 
        return true; 
    } else {
        logSystem("Status: OK (Dust:" + String(status.dust,0) + 
                " Days:" + String(daysSinceLastClean) + ")");
        return false; 
    }
}