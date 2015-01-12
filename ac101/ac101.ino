#include <Sensor.h>
#include <ACS712.h>
 
#define CURRENT_ON_OFF_PIN 8
#define ACS712_ANALOG_PIN 0
#define ACS712_VCC_PIN 9
#define FREQUENCY 60.0
 
#define MAX_VALUES 100
 
int valuesInt[MAX_VALUES];
float values[MAX_VALUES];
 
ACS712 acs712;
Sensor &currentSensor = acs712;
 
void setup() {
  Serial.begin(57600);
  Serial.println("====== ACS712 Sensor Demo ======");
  Serial.println("Assumption: ACS712 on A0 and D9, current on/off on D8");
  acs712.initialize(ACS712_ANALOG_PIN, ACS712_VCC_PIN);
  pinMode(CURRENT_ON_OFF_PIN, OUTPUT);
  digitalWrite(CURRENT_ON_OFF_PIN, LOW);
}
 
void loop() {
  int offset;
  Serial.print("*****Start with offset Calibration");
  dotDelay(2);
  takeMeasurement(500);
  Serial.println("done taking sample.");
  Serial.print("Average: ");
  offset = average();
  Serial.println(offset);
  //acs712.setCalibration(offset - 512 + 5);
  Serial.println("**** Done calibrating");
  
  Serial.print("***** Measuring w/o current");
  dotDelay(2);
  takeMeasurement(offset);
  Serial.println("done");
  printValues();
    
  Serial.print("***** Measuring with current (wait for current to stabilize)");
  digitalWrite(CURRENT_ON_OFF_PIN, HIGH);
  dotDelay(10);
  takeMeasurement(offset);
  Serial.println("done");
  printValues();
  
  Serial.print("Demo completed");
  dotDelay(4);
  Serial.println("Goodby");
  digitalWrite(CURRENT_ON_OFF_PIN, LOW);
  
  // Wait forever
  while(1) {
    delay(10000);
  }
  
}
 
 
int takeMeasurement(int init) {
  int lastValue = init;
  int count = 0;
  
  currentSensor.beginSampling();
  for(int i = 0; (i < MAX_VALUES * 10) && (count < MAX_VALUES); i++) {
    currentSensor.readSensor();
    valuesInt[count] = currentSensor.getIntegerValue();
    if(abs(valuesInt[count] - lastValue) < 15) {
      lastValue = currentSensor.getIntegerValue();
      values[count++] = currentSensor.getFloatValue();
    }
    delayMicroseconds((1000000.0/FREQUENCY) / (MAX_VALUES / 2));
  }
  currentSensor.endSampling();
  return count;
}
 
void printValues() {
  float minValue = 2000;
  float maxValue = -1;
  float averageValue = 0;
    
  Serial.print(values[0]);
  for(int i = 1; i < MAX_VALUES; i++) {
    Serial.print(", ");
    Serial.print(values[i]);
    if(values[i] < minValue) minValue = values[i];
    if(values[i] > maxValue) maxValue = values[i];
    averageValue += values[i];
  }
  Serial.println(" ");
  Serial.print("Min: ");
  Serial.print(minValue);
  Serial.print(" Max: ");
  Serial.print(maxValue);
  Serial.print(" Average: ");
  Serial.println(averageValue / MAX_VALUES);
  Serial.print("Delta MM: ");
  Serial.println(maxValue - minValue);
}
 
 
int average() {
  float averageValue;
  int count = 1;
  
  for(int i = 0; i < MAX_VALUES; i++) {
    if(abs(valuesInt[i] - 512) < 50) {
      averageValue += valuesInt[i];
      count++;
    }
  }
  
  return round(averageValue / count);
}
  
  
 
void dotDelay(int t) {
  for(int i = 0; i < t; i++) {
    Serial.print(".");
    delay(1000);
  }
}
 
