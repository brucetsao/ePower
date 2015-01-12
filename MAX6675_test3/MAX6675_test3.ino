#include <max6675.h>

#include <Wire.h>

int thermoDO = 8;
int thermoCS = 9;
int thermoCLK = 10;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;


void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  pinMode(vccPin, OUTPUT); 
  digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); 
  digitalWrite(gndPin, LOW);
  
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp


  Serial.print("Now Temperature is :");
  Serial.print(thermocouple.readCelsius());
  Serial.print("oC  ");
  Serial.print(thermocouple.readFahrenheit());

  Serial.print("oF");
  Serial.print("\n");
  delay(1000);
}
