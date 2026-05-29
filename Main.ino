#include "Sensor/Sensor.h"
#include "Display/Display.h"

#define BUTTON_PIN REPLACE

// Wifi configuration
#define WIFI_SSID REPLACE
#define WIFi_PASS REPLACE

// Google Spreadsheet API token
#define GAS_API REPLACE

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
    Serial.println("Sensor ready");

    // Button
    pinMode(BUTTON_PIN, INPUT);
    Serial.println("Button ready");

    // Display ST7735
    ST7735_Setup();
    Serial.println("Display ready");

    // Waiting for user
    ST7735_Text_Center("Tekan tombol untuk memulai");
    while (digitalRead(BUTTON_PIN) == LOW);
}

static void
loop()
{
    // Update button state
    buttonState = digitalRead(BUTTON_PIN);

    // Getting values from sensor MAX30102
    bpm = MAX30102_HeartRate();
    spo2 = MAX30102_SPO2();

    // Debugging
    Serial.print("BPM = ");
    Serial.print(bpm);
    Serial.print(" SPO2 = ");
    Serial.println(spo2);
}
