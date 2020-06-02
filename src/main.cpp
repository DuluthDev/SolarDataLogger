#include <Arduino.h>
//#include <DallasTemperature.h>
//#include <OneWire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

//#define ONE_WIRE_BUS 14

//OneWire oneWire(ONE_WIRE_BUS);

//DallasTemperature sensors(&oneWire);

#define msfPIN 53

#define DHTPIN 2

#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

const int controlPin[16] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37}; // define pins

const int triggerType = LOW;// your relay type
int loopDelay = 1000;// delay in loop
int readDelay = 50;

void setup() {
  Serial.begin(9600);
  Serial.println();

  dht.begin();
  ina219.begin();

  Serial.print("Pin");
  Serial.print("Intensity,");
  Serial.print("Celcius,");
  Serial.print("Humidity,");
  Serial.print("Bus Voltage(V),");
  Serial.print("Shunt Voltage(mV),"); 
  Serial.print("Load Voltage(V),"); 
  Serial.print("Current(mA),"); 
  Serial.print("Power(mW),"); 
  Serial.print("Shorted Bus Voltage(V),");
  Serial.print("Shorted Shunt Voltage(mV),"); 
  Serial.print("Shorted Load Voltage(V),"); 
  Serial.print("Shorted Current(mA),"); 
  Serial.print("Shorted Power(mW)"); 
  Serial.println();
} 

void loop() {

  // Establish inputs
  
  unsigned int lightIntensity;
  lightIntensity = analogRead(A14);
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  sensors_event_t event;    
  dht.temperature().getEvent(&event);
  dht.humidity().getEvent(&event);

  // Collect Data
  for(int i=0; i<16; i++)  {
    delay(readDelay);
    pinMode(controlPin[i], OUTPUT);     // set pin as output
    digitalWrite(controlPin[i], LOW);   // set initial state OFF for low trigger relay
      Serial.print(controlPin[i]);
      Serial.print(",");
      Serial.print(lightIntensity);
      Serial.print(",");
      Serial.print(event.temperature);
      Serial.print(",");
      Serial.print(event.relative_humidity);
      Serial.print(",");
      Serial.print(busvoltage);
      Serial.print(",");
      Serial.print(shuntvoltage);
      Serial.print(",");
      Serial.print(loadvoltage);
      Serial.print(",");
      Serial.print(current_mA);
      Serial.print(",");
      Serial.print(power_mW);
      Serial.print(",");
      delay(readDelay);
      digitalWrite(msfPIN, LOW);
      // Get Shorted Info
        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        power_mW = ina219.getPower_mW();
        loadvoltage = busvoltage + (shuntvoltage / 1000);
        Serial.print(busvoltage);
        Serial.print(",");
        Serial.print(shuntvoltage);
        Serial.print(",");
        Serial.print(loadvoltage);
        Serial.print(",");
        Serial.print(current_mA);
        Serial.print(",");
        Serial.print(power_mW);
        Serial.println("");
      digitalWrite(msfPIN, HIGH);
      delay(readDelay);
      //  sensors.requestTemperatures();
      //  Serial.println(sensors.getTempCByIndex(0));
      digitalWrite(controlPin[i], HIGH);  // set initial state OFF for high trigger relay     
    delay(loopDelay);
  }
}
