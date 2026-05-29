/*

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
*/

#include "Sensor.h"

static MAX30105 particleSensor;

static uint32_t irBuffer[100]; //infrared LED sensor data
static uint32_t redBuffer[100];  //red LED sensor data

// Heart rate
static const byte RATE_SIZE = 10; //Increase this for more averaging. 4 is good.
static byte rates[RATE_SIZE]; //Array of heart rates
static byte rateSpot = 0;
static long lastBeat = 0; //Time at which the last beat occurred
static float beatsPerMinute = 0;
static float filteredBPM = 0;
static int beatAvg;

// SPO2
static int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
static int32_t spo2 = 0; //SPO2 value
static float filteredSPO2 = 0;

void
MAX30102_Setup()
{
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    return;
  }

  byte ledBrightness = 50; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(
    ledBrightness,
    sampleAverage,
    ledMode,
    sampleRate,
    pulseWidth,
    adcRange
  ); //Configure sensor with these settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

int
MAX30102_HeartRate()
{
  long irValue = particleSensor.getIR();

  if (!checkForBeat(irValue) || irValue < 50000)
    return 0;

  //We sensed a beat!
  long delta = millis() - lastBeat;
  lastBeat = millis();

  if (delta <= 0)
    return 0;

  beatsPerMinute = 60.0 / (delta / 1000.0);

  if (beatsPerMinute > 45 && beatsPerMinute < 130) {
    rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
    rateSpot %= RATE_SIZE; //Wrap variable

    //Take average of readings
    beatAvg = 0;
    for (byte x = 0 ; x < RATE_SIZE ; x++)
      beatAvg += rates[x];
    beatAvg /= RATE_SIZE;

    filteredBPM = (filteredBPM * 0.90) + (beatAvg * 0.10);
  }

  if (filteredBPM <= 0)
    return 0;

  return constrain((int)filteredBPM, 50, 130);
}
  
int32_t
MAX30102_SPO2()
{
  static byte bufferIndex = 0;
  static bool bufferFilled = false;

  particleSensor.check(); //Check the sensor for new data
  while (particleSensor.available()) {
    redBuffer[bufferIndex] = particleSensor.getRed();
    irBuffer[bufferIndex] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

    bufferIndex++;
    if (bufferIndex >= 100) {
      bufferIndex = 0;
      bufferFilled = true;
    }
  }

  if (!bufferFilled)
    return 0;

  int32_t dummyHeartRate;
  int8_t dummyValidHeartRate;

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(
    irBuffer,
    100,
    redBuffer,
    &spo2,
    &validSPO2,
    &dummyHeartRate,
    &dummyValidHeartRate
  );

  if (!validSPO2)
    return 0;

  filteredSPO2 = (filteredSPO2 * 0.92) + (spo2 * 0.08);

  if (filteredSPO2 <= 0)
    return 0;
  
  return constrain((int)filteredSPO2, 85, 100);
}