#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

Adafruit_INA219 ina219;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display;

DHT dht(2, DHT22);

const int controlPin[16] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}; // define pins

const int triggerType = LOW; // your relay type
int loopDelay = 1000;        // delay in loop
int readDelay = 50;

int updateDisplay(int pin, int light, float volt, float amp, int temp, int humidity)
{
  // Prep DisplayQ
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Pin: ");
  display.println(pin);
  display.setCursor(64, 0);
  display.print("Light: ");
  display.print(light);
  display.setCursor(0, 10);
  display.print("V's: ");
  display.print(volt);
  display.setCursor(64, 10);
  display.print("mA's: ");
  display.print(amp);
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temp);
  display.print("c");
  display.setCursor(64, 20);
  display.print("Humid: ");
  display.print(humidity);
  display.print("%");
  display.display();
};

int logData(int iteration, int pin, float light, float temp, float humidity, float volt, float amp)
{
  Serial.print(iteration);
  Serial.print(",");
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

int iteration = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println();

  dht.begin();
  ina219.begin();

  Serial.print("Iteration,");
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
  display.setCursor(64,32);
  display.setTextSize(1);
  display.println("Loading...");
  delay(1000);
  display.display();
}

void loop()
{

  // Establish inputs
  unsigned int lightIntensity;
  float voltage_mV = 0;
  float current_mA = 0;

  float humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  // Collect Data
  for (int i = 0; i < 16; i++)
  {
    // Track Iterations
    iteration++;

    lightIntensity = analogRead(A1);

    voltage_mV = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();

    // Output Data

    //pinMode(controlPin[i], OUTPUT);   // set pin as output
    // digitalWrite(controlPin[i], LOW); // set initial state OFF for low trigger relay
    
    // Log data to Serial interface
    logData(iteration, controlPin[i], lightIntensity, temperature, humidity, voltage_mV, current_mA);

    // Update Display
    updateDisplay(controlPin[i], lightIntensity, voltage_mV, current_mA, temperature, humidity);

    // digitalWrite(controlPin[i], HIGH); // set initial state OFF for high trigger relay
  delay(loopDelay);
  }
}
