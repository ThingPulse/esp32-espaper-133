#pragma once

#include <Arduino.h>
#include "driver/adc.h"
#include "settings.h"

#define MATH_MIN(a,b) ((a)<(b)?(a):(b))
#define MATH_MAX(a,b) ((a)>(b)?(a):(b))

void initBatteryMonitor() {

}

float getBatteryVoltage() {
    uint16_t adcMeasurement = analogRead(BAT_MON_PIN);
    float rawVoltage = adcMeasurement * 3.3 / 4095.0;
    float batteryVoltage =  0.25 + rawVoltage * (BAT_R1 + BAT_R2) / BAT_R2;
    log_v("Raw: %d, Battery: %fV", rawVoltage, batteryVoltage);
    return batteryVoltage;
}

float getBatteryPercentage() {

    float voltage = getBatteryVoltage();
    float maxVoltage = 4.200f;
    float minVoltage = 2.8f;
    return MATH_MAX(MATH_MIN((voltage - minVoltage) / (maxVoltage - minVoltage), 1.0), 0.0) * 100.0f;

}

void deinitBatteryMonitor() {
    pinMode(BAT_MON_PIN, INPUT);
    //adc_power_release();

}