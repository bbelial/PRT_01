#include "Sensor/Sensor.h"

#define BUTTON_PIN

// Wifi configurations
static const char* ssid = "";
static const char* passphrase = "";

// Google Spreadsheet API token
static const char* GASAPI = "AKfycbzwmoYcl12LmyeP9f-hCk0m4aAbFIX8IOlb50QexMDfdS82uSJSWCOXzj61FcA51dH-nA";

// Heart rate and SPO2 definition
static int bpm;
static int32_t spo2;

// Button state
static int buttonState = 0;

static void
setup()
{
    Serial.begin(115200);
    Serial.println("Initializing...");
    
    // Sensor MAX30102
    MAX30102_Setup();
    Serial.println("Sensor detected");

    // Button 
    pinMode(BUTTON_PIN, INPUT);
}

static void
loop()
{
    // Update button state
    buttonState = digitalRead(BUTTON_PIN);

    // Getting values from sensor MAX30102
    bpm = MAX30102_HeartRate();
    spo2 = MAX30102_SPO2();

}
