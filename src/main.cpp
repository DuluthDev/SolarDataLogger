//#include <Arduino.h>
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
Adafruit_SSD1306 display;

#define DHTPIN 2

#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

const int controlPin[16] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}; // define pins

const int triggerType = LOW; // your relay type
int loopDelay = 1000;        // delay in loop
int readDelay = 50;

int updateDisplay(int pin, float light, float volt, float amp)
{
  // Prep DisplayQ
  display.clearDisplay();
  display.println("Pin: ");
  display.println("Light Intensity: ");
  display.println("Voltage(mV): ");
  display.println("Amperage(mA): ");
  display.setCursor(105, 0);
  display.println(pin);
  display.println(light);
  display.println(volt);
  display.println(amp);
  display.display();
};

int logData(int pin, float light, float temp, float humidity, float volt, float amp)
{
  Serial.print(pin);
  Serial.print(",");
  Serial.print(light);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(volt);
  Serial.print(",");
  Serial.print(amp);
  Serial.print(",");
  Serial.print((amp * volt));
  Serial.println();
};

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

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return; // exit
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.setTextWrap(false);
  display.display();
}

void loop()
{

  // Establish inputs
  unsigned int lightIntensity;
  lightIntensity = analogRead(A1);

  float voltage_mV = 0;
  float current_mA = 0;
  voltage_mV = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);

  // Collect Data
  for (int i = 0; i < 2; i++)
  {
    pinMode(controlPin[i], OUTPUT);   // set pin as output
    // digitalWrite(controlPin[i], LOW); // set initial state OFF for low trigger relay
    
    // Log data to Serial interface
    logData(controlPin[i], lightIntensity, event.temperature, event.relative_humidity, voltage_mV, current_mA);

    // Update Display
    updateDisplay(controlPin[i], lightIntensity, voltage_mV, current_mA);

    // digitalWrite(controlPin[i], HIGH); // set initial state OFF for high trigger relay
  }
  delay(loopDelay);
}
