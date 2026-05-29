#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <MAX30105.h>
#include <spo2_algorithm.h>
#include <heartRate.h>

void MAX30102_Setup();
int MAX30102_HeartRate();
int32_t MAX30102_SPO2();

#endif