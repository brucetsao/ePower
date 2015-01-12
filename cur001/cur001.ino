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



//----------------AC  Current Measure  ASC712 --------------

void setup() {
 
  Serial.begin(9600);
  
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
   
   
}
 
void loop() {
 
  float average = 0;
  for(int i = 0; i < 1000; i++) {
    average = average + (.0264 * analogRead(current01) -13.51) / 1000;
    delay(1);
  }
  Serial.println(average);  
}
