#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_INA219 ina219;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display;

DHT dht(2, DHT22);

const int controlPin[16] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37}; // define relay pins

const int triggerType = LOW; // relay type

int countDownAmnt = 59;       // countdown delay in loop
int s_RelayPin = 12;
int iteration = 0;

int updateDisplay(int pin, int light, float volt, float amp, int temp, int humidity)
{
  // Prep DisplayQ
  display.clearDisplay();
  display.setCursor(0, 5);
  display.print("Pin: ");
  display.println(pin);
  display.setCursor(64, 5);
  display.print("Light: ");
  display.print(light);
  display.setCursor(0, 15);
  display.print("V's: ");
  display.print(volt);
  display.setCursor(64, 15);
  display.print("mA's: ");
  display.print(amp);
  display.setCursor(0, 25);
  display.print("Temp: ");
  display.print(temp);
  display.print("c");
  display.setCursor(64, 25);
  display.print("Humid: ");
  display.print(humidity);
  display.print("%");
  display.display();
};

int logData(int iteration, int pin, int light, float temp, float humidity, float volt, float amp)
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

int countdownTimer(int countDown, float volt, float amps)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Next Scan in: ");
  display.setCursor(50,10);
  display.setTextSize(2);
  display.println(countDown);
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print("V: ");
  display.print(volt);
  display.setCursor(64,25);
  display.print("mA: ");
  display.print(amps);
  display.display();

};

void setup()
{
  Serial.begin(9600);
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
  display.setCursor(64, 32);
  display.setTextSize(1);
  display.println("Loading...");
  display.display();

  dht.begin();
  ina219.begin();

  Serial.print("Iteration,");
  Serial.print("Pin,");
  Serial.print("Light Intensity,");
  Serial.print("Temp C,");
  Serial.print("Humidity,");
  Serial.print("Voltage(V),");
  Serial.print("Amperage(mA),");
  Serial.print("Power(mW)");
  Serial.println();
}

void loop()
{
  // Establish inputs
  unsigned int lightIntensity;
  float voltage_V = 0;
  float s_current_mA = 0;

  float humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  // Collect Data
  for (int i = 0; i < 16; i++)
  {
    // Track Iterations
    iteration++;

    // pinMode(controlPin[i], OUTPUT);   // set control pin as output
    // digitalWrite(controlPin[i], LOW); // set initial state OFF for low trigger relay

    lightIntensity = analogRead(A1);
    voltage_V = ina219.getBusVoltage_V();

    // Record Shorted Current
    pinMode(s_RelayPin, HIGH);
    delay(500);
    s_current_mA = ina219.getCurrent_mA();
    pinMode(s_RelayPin, LOW);
    delay(500);

    // Output Data
    // Log data to Serial interface
    logData(iteration, controlPin[i], lightIntensity, temperature, humidity, voltage_V, s_current_mA);
    // Update Display
    updateDisplay(controlPin[i], lightIntensity, voltage_V, s_current_mA, temperature, humidity);

    // digitalWrite(controlPin[i], HIGH); // Turn off relay

  }
  
  // Run countdown delay
  int countDown = countDownAmnt;
  while (countDown >= 0)
  {
    countdownTimer(countDown, voltage_V, s_current_mA);
    delay(1000);
    countDown--;
  }
}
