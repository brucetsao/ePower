#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
    //  Here is where i cycle the clock's power
    pinMode(4,OUTPUT);
    digitalWrite(4,LOW);
    delay(10);
    digitalWrite(4,HIGH);
    delay(10);
    
    // Everything else is from RTClib example
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();;

    //RTC.adjust(DateTime(__DATE__, __TIME__));

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
}

void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    
    Serial.println();
    delay(1000);
}
