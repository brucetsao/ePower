
//----------------reklay set

#define relay01 8
#define relay02 9
#define relay03 10
#define relay04 11
#define turnoff  LOW
#define turnon  HIGH
//----------------reklay set

//----------------AC  Current Measure  ASC712 --------------
#define current01  A1
#define current02  A2
#define current03  A3



//----------------AC  Current Measure  ASC712 -------------
/*


     long adc_raw = analogRead(currentPin) - adc_zero;
     currentAcc += (unsigned long)(adc_raw * adc_raw);
     
     */

const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
const int adc_zero = 510;                                                     // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)

void setup()
{
    Serial.begin(9600);
   Serial.println("Relay test ");
   pinMode(relay01,OUTPUT) ;
   pinMode(relay02,OUTPUT) ;
   pinMode(relay03,OUTPUT) ;
   pinMode(relay04,OUTPUT) ;
   digitalWrite(relay01,turnon) ;
   digitalWrite(relay02,turnon) ;
   digitalWrite(relay03,turnon) ;
   digitalWrite(relay04,turnon) ;
   
 Serial.begin(9600);
}

void loop()
{
 unsigned long currentAcc = 0;
 unsigned int count = 0;
 unsigned long prevMicros = micros() - sampleInterval ;
 while (count < numSamples)
 {
   if (micros() - prevMicros >= sampleInterval)
   {
     int adc_raw = analogRead(current01) - adc_zero;
     currentAcc += (unsigned long)(adc_raw * adc_raw);
     ++count;
     prevMicros += sampleInterval;
   }
 }
 
 float rms = sqrt((float)currentAcc/(float)numSamples) * (75.7576 / 1024.0);

 Serial.print(rms);
 Serial.print("/");
 Serial.print(analogRead(current01));
 Serial.print("/");
 Serial.print(analogRead(current02));
 Serial.print("/");
 Serial.print(analogRead(current03));
 Serial.print("\n");

}

