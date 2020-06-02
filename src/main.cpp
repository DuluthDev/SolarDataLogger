#include <Arduino.h>
//#include <DallasTemperature.h>
//#include <OneWire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_INA219 ina219;

//#define ONE_WIRE_BUS 14

//OneWire oneWire(ONE_WIRE_BUS);

//DallasTemperature sensors(&oneWire);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2

#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

const int controlPin[16] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}; // define pins

const int triggerType = LOW; // your relay type
int loopDelay = 1000;        // delay in loop
int readDelay = 50;

void setup()
{
  Serial.begin(9600);
  Serial.println();

  dht.begin();
  ina219.begin();

  Serial.print("Pin,");
  Serial.print("Light Intensity,");
  Serial.print("Temp C,");
  Serial.print("Humidity,");
  Serial.print("Voltage(mV),");
  Serial.print("Amperage(mA),");
  Serial.print("Power(mW)");
  Serial.println();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    return; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.print("Pin,");
  display.print("Light Intensity,");
  display.print("Temp C,");
  display.print("Humidity,");
  display.print("Voltage(mV),");
  display.print("Amperage(mA),");
  display.print("Power(mW)");
  display.println();
}

void loop()
{

  // Establish inputs
  unsigned int lightIntensity;
  lightIntensity = analogRead(A14);
  int voltage_mV = analogRead(A0);
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);

  // Collect Data
  delay(readDelay);
  for (int i = 0; i < 2; i++)
    {
      pinMode(controlPin[i], OUTPUT);   // set pin as output
      digitalWrite(controlPin[i], LOW); // set initial state OFF for low trigger relay
      Serial.print(controlPin[i]);
      Serial.print(",");
      Serial.print(lightIntensity);
      Serial.print(",");
      Serial.print(event.temperature);
      Serial.print(",");
      Serial.print(event.relative_humidity);
      Serial.print(",");
      Serial.print(voltage_mV);
      Serial.print(",");
      Serial.print(current_mA);
      Serial.print(",");
      Serial.print((current_mA * voltage_mV));
      Serial.println();

      digitalWrite(controlPin[i], HIGH); // set initial state OFF for high trigger relay
    }
  delay(loopDelay);
}
