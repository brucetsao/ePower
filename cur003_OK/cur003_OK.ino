const int currentPin = 1;
const unsigned long sampleTime = 100000UL;                         
const unsigned long numSamples = 250UL;                               
const unsigned long sampleInterval = sampleTime/numSamples; 
const int adc_zero = 510;                                                     

void setup()
{
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
      int adc_raw = analogRead(currentPin) - adc_zero;
      currentAcc += (unsigned long)(adc_raw * adc_raw);
      ++count;
      prevMicros += sampleInterval;
    }
  }
  
  float rms = sqrt((float)currentAcc/(float)numSamples) * (75.7576 / 1024.0);
  Serial.println(rms);
}
