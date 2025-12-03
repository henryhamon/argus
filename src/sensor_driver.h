#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Arduino.h>
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>
#include "config.h"

// Initializes all sensors (DHT, I2C, Light, Dust Pins)
void initSensors();

// Wrapper to read temperature safely (returns NaN on error)
float readTemperature();

// Wrapper to read humidity safely
float readHumidity();

// Wrapper to read light intensity
float readLightLevel();

// Complex function: Handles LED switching, timing, ADC reading, and smoothing
float readDustSensorSmooth();

// Simulation Setters (Used by Serial Command parser)
void setSimTemp(float v);
void setSimHum(float v);
void setSimLux(float v);
void setSimDust(float v);

#endif