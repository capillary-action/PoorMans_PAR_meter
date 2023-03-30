/* VEML7700 and TSL2591 Light Sensor Example */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#include "Adafruit_VEML7700.h"
#include <Adafruit_AS7341.h>

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_AS7341 as7341;
// Reference spectrum for PAR calculation (values taken from McCree, 1972)
const float PAR_REFERENCE_SPECTRUM[8] = {0.0021, 0.0067, 0.0600, 0.1693, 0.2558, 0.2824, 0.2720, 0.1655};

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  Serial.println("VEML7700, TSL2591, AS7341 Light Sensor Test");

  if (!tsl.begin()) {
    Serial.println("No TSL2591 sensor detected");
    while (1);
  }
  Serial.println("TSL2591 sensor detected");

  if (!veml.begin()) {
    Serial.println("No VEML7700 sensor detected");
    while (1);
  }
  if (!as7341.begin()){
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }

  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);
}

void loop() {
  uint16_t readings[12];
  if (!as7341.readAllChannels(readings)){
    Serial.println("Error reading all channels!");
    return;
  }
  
  uint32_t sum_readings = 0;
  for (int i = 0; i < 12; i++) {
    sum_readings += readings[i];
  }
  
  float normalized_power[8];
  for (int i = 0; i < 8; i++) {
    normalized_power[i] = readings[i] / (float)sum_readings;  // Normalize by sum of all channels
    Serial.print("Normalized spectral power at ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(normalized_power[i]);
  }    
  
  /* Read lux using automatic method, specify VEML_LUX_AUTO */
  float lux_veml = veml.readLux(VEML_LUX_AUTO);
  Serial.print("VEML7700 Lux: ");
  Serial.println(lux_veml);
  Serial.println("------------------------------------");
  Serial.println("VEML7700 Settings used for reading:");
  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }
  tsl.setGain((tsl2591Gain_t)veml.getGain());
  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }
  tsl.setTiming((tsl2591IntegrationTime_t)veml.getIntegrationTime());
  Serial.println("------------------------------------");
  Serial.println("");

  /* Read full spectrum and infrared light levels */
  uint32_t full = tsl.getFullLuminosity();
  uint16_t ir = tsl.getLuminosity(TSL2591_INFRARED);
  /* Calculate the visible light level and lux */
  float lux_tsl = tsl.calculateLux(full, ir);
  Serial.print("TSL2591 Lux: ");
  Serial.println(lux_tsl);
  Serial.println("------------------------------------");
  Serial.println("");

  float parV = 0;
  for (int i = 0; i < 8; i++) {
    parV += normalized_power[i] * lux_veml * PAR_REFERENCE_SPECTRUM[i];
  }
    float parT = 0;
  for (int i = 0; i < 8; i++) {
    parT += normalized_power[i] * lux_tsl * PAR_REFERENCE_SPECTRUM[i];
  }
  Serial.print("PARv value: ");
  Serial.println(parV);
  Serial.print("PARt value: ");
  Serial.println(parT);
  

  delay(3000);
}
