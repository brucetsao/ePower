
// These constants won't change.  They're used to give names
// to the pins used:
//const int SW1analogInPin = A0;  // Analog input pin that the potentiometer is attached to
#include <DHT22.h>
#include <stdio.h>
#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 2

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
const int analogOutPin = 11; // Analog output pin that the LED is attached to
const float RMSValue = 0.354; //2^(1/2)/2
#define SW1  12
#define SW2  8 
#define SW3  7 
#define SW1analogInPin A0
#define SW2analogInPin A1
#define SW3analogInPin A2

unsigned short uiSW1CurrentValue = 0;
unsigned short uiSW2CurrentValue = 0;
unsigned short uiSW3CurrentValue = 0;
unsigned short uiTemperature=0;
unsigned short uiHuminity=0;
unsigned int uiCurrentMaxValue = 0;
unsigned int uiCurrentMinValue = 65535;
unsigned char uSwStartTime[3][2];
unsigned char uSwStopTime[3][2];


unsigned short uiLightVal=0;

void _check_UartCmd(void)
{
   int i;
   //RXbuff存放arduino接收到的資料。
   //TXBuff:arduino將要送到手機的資料放到TXBuff，在一次送出去
   char RxBuf[10],TxBuf[20];
   //擷取系統內，serial是否有資料，若有資料，lenth會大0
   int length = Serial.available();

  if(length>0)
  {
    //讀出資料，放入RxBuf
     Serial.readBytes( RxBuf,length);
     switch(RxBuf[0])
     {
     //確認接收的RxBuf[0] ，1=sw1,2=sw2,3=sw3,4=sw4
         case('1'):
           //確認接收的RxBuf[1] ，是否為0，1設定on,0設定off
           if(RxBuf[1] == '0')
           {
              digitalWrite(SW1, LOW);
           }
           else
           {
              digitalWrite(SW1, HIGH);
           }
           for(int i=0;i<2;i++)
           {
             TxBuf[i] = RxBuf[i];
           }
           Serial.write(TxBuf,2);
           break;
         case('2'):
           //確認接收的RxBuf[1] ，是否為0，1設定on,0設定off
           if(RxBuf[1] == '0')
           {
              digitalWrite(SW2, LOW);
           }
           else
           {
              digitalWrite(SW2, HIGH);
           }
           for(int i=0;i<2;i++)
           {
             TxBuf[i] = RxBuf[i];
           }
           Serial.write(TxBuf,2);
           break;
         case('3'):
           //確認接收的RxBuf[1] ，是否為0，1設定on,0設定off
           if(RxBuf[1] == '0')
           {
              digitalWrite(SW3, LOW);
           }
           else
           {
              digitalWrite(SW3, HIGH);
           }
           for(int i=0;i<2;i++)
           {
             TxBuf[i] = RxBuf[i];
           }
           Serial.write(TxBuf,2);
           break;
         case('5')://讀取所有的AD資料。
           TxBuf[0] = RxBuf[0];
           TxBuf[1] = uiSW1CurrentValue%0x100;
           TxBuf[2] = uiSW1CurrentValue/0x100;
           TxBuf[3] = uiSW2CurrentValue%0x100;
           TxBuf[4] = uiSW2CurrentValue/0x100;
           TxBuf[5] = uiSW3CurrentValue%0x100;
           TxBuf[6] = uiSW3CurrentValue/0x100;
           TxBuf[7] = uiLightVal%0x100;
           TxBuf[8] = uiLightVal/0x100;
           TxBuf[9] = uiTemperature%0x100;
           TxBuf[10] = uiTemperature/0x100;
           TxBuf[11] = uiHuminity%0x100;
           TxBuf[12] = uiHuminity/0x100;
           Serial.write(TxBuf,13);
           break;
         case('6')://read now time
           {
           DateTime now = rtc.now();
           TxBuf[0] = RxBuf[0];
           TxBuf[1] = (char)(now.year()-2000);
           TxBuf[2] = (char)(now.month());
           TxBuf[3] = (char)(now.day());
           TxBuf[4] = (char)(now.hour());
           TxBuf[5] = (char)(now.minute());
           TxBuf[6] = (char)(now.second());
           Serial.write(TxBuf,7);
           }
           break;
         case('7')://Set now time
           {
           rtc.adjust(DateTime((int)RxBuf[1]+2000, (int)RxBuf[2], (int)RxBuf[3], (int)RxBuf[4], (int)RxBuf[5], (int)RxBuf[6]));
           DateTime now = rtc.now();
           TxBuf[0] = RxBuf[0];
           TxBuf[1] = (char)(now.year()-2000);
           TxBuf[2] = (char)(now.month());
           TxBuf[3] = (char)(now.day());
           TxBuf[4] = (char)(now.hour());
           TxBuf[5] = (char)(now.minute());
           TxBuf[6] = (char)(now.second());
           Serial.write(TxBuf,7);
           }
           break;
         case('8')://Set switch start stop time
           {
             int iIndex = (RxBuf[1]);
             uSwStartTime[ iIndex ][0] = (unsigned char)RxBuf[2];//start time hour
             uSwStartTime[ RxBuf[1] ][1] = RxBuf[3];//start time minutes
             uSwStopTime[ RxBuf[1] ][0] = RxBuf[4];//stop time hour
             uSwStopTime[ RxBuf[1] ][1] = RxBuf[5];//stop time minutes
             TxBuf[0] = RxBuf[0];
             TxBuf[1] = RxBuf[1];//switch ?
             TxBuf[2] = uSwStartTime[ RxBuf[1] ][0];//start hour
             TxBuf[3] = uSwStartTime[ RxBuf[1] ][1];//start minutes
             TxBuf[4] = uSwStopTime[ RxBuf[1] ][0];//stop hour
             TxBuf[5] = uSwStopTime[ RxBuf[1] ][1];//stop minutes
             Serial.write(TxBuf,6);
           }
           break;
           
     }
     
     //清除接收buffer。
     length=0;
     for(int i=0;i<10;i++)
     {
       RxBuf[i] = 0;
     }
     
  }
}


void _read_Current()
{

  unsigned int sensorValue=0;
  uiCurrentMaxValue = 0;
  uiCurrentMinValue = 65535;
  
  
  //讀取一千次，取得最高與最低值，相減得到峰對峰值，(乘上根號2得到RMS換算電流，在乘上系數得到功率值。此部分在手機程式上完成)
  for(int i=0;i<1000;i++)
  {
    sensorValue = analogRead(SW1analogInPin);            
    if(sensorValue > uiCurrentMaxValue)
    {
      uiCurrentMaxValue = sensorValue;
    }
    if(sensorValue < uiCurrentMinValue)
    {
      uiCurrentMinValue = sensorValue;
    }
  }
  uiSW1CurrentValue = uiCurrentMaxValue-uiCurrentMinValue;
  uiCurrentMaxValue = 0;
  uiCurrentMinValue = 65535;
  
  //讀取一千次，取得最高與最低值，相減得到峰對峰值，(乘上根號2得到RMS換算電流，在乘上系數得到功率值。此部分在手機程式上完成)
  for(int i=0;i<1000;i++)
  {
    sensorValue = analogRead(SW2analogInPin);            
    if(sensorValue > uiCurrentMaxValue)
    {
      uiCurrentMaxValue = sensorValue;
    }
    if(sensorValue < uiCurrentMinValue)
    {
      uiCurrentMinValue = sensorValue;
    }
  }
  uiSW2CurrentValue = uiCurrentMaxValue-uiCurrentMinValue;
  uiCurrentMaxValue = 0;
  uiCurrentMinValue = 65535;

  //讀取一千次，取得最高與最低值，相減得到峰對峰值，(乘上根號2得到RMS換算電流，在乘上系數得到功率值。此部分在手機程式上完成)
  for(int i=0;i<1000;i++)
  {
    sensorValue = analogRead(SW3analogInPin);            
    if(sensorValue > uiCurrentMaxValue)
    {
      uiCurrentMaxValue = sensorValue;
    }
    if(sensorValue < uiCurrentMinValue)
    {
      uiCurrentMinValue = sensorValue;
    }
  }
  uiSW3CurrentValue = uiCurrentMaxValue-uiCurrentMinValue;
}

void _read_LightVal()
{
  uiLightVal = analogRead(A3);
}

void _read_Temp_Hum(void)
{ 
  float fTemp=0;
  DHT22_ERROR_t errorCode;
  
  //擷取DHT22的值
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      uiTemperature = myDHT22.getTemperatureCInt();
      uiHuminity = myDHT22.getHumidityInt();
      break;
    case DHT_ERROR_CHECKSUM:
      break;
    case DHT_BUS_HUNG:
      break;
    case DHT_ERROR_NOT_PRESENT:
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      break;
    case DHT_ERROR_TOOQUICK:
      break;
  }
}

void _check_SwOn_Time()
{
  DateTime now = rtc.now();
  for(int i=0;i<3;i++)
  {
    if( now.hour() == uSwStartTime[i][0] && now.minute() == uSwStartTime[i][1] )
    {
      if(now.second()<=3)
      {
        if(i==0)
          digitalWrite(SW1, HIGH);
        else if(i==1)
          digitalWrite(SW2, HIGH);
        else if(i==2)
          digitalWrite(SW3, HIGH);
      }
    }
    if( now.hour() == uSwStopTime[i][0] && now.minute() == uSwStopTime[i][1] )
    {
      if(now.second()<=3)
      {
        if(i==0)
          digitalWrite(SW1, LOW);
        else if(i==1)
          digitalWrite(SW2, LOW);
        else if(i==2)
          digitalWrite(SW3, LOW);
      }
    }
  }
}

void _check_SwOn_Condition()
{
  if( uiTemperature >= 250)
  {
    digitalWrite(SW1, HIGH);
  }
  else
  {
    digitalWrite(SW1, LOW);
  }
  
  if(uiLightVal>=500)
  {
    digitalWrite(SW3, HIGH);
  }
  else
  {
    digitalWrite(SW3, LOW);
  }
  
  
}


void setup() {
  // 設定藍芽baudrate 9600 bps:
  Serial.begin(9600); 
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  
  pinMode(SW1, OUTPUT);
  digitalWrite(SW1, LOW);
  pinMode(SW2, OUTPUT);
  digitalWrite(SW2, LOW);
  pinMode(SW3, OUTPUT);
  digitalWrite(SW3, LOW);

}



void loop() 
{
  //由ACS712讀取電流值
  _read_Current();
  
  //從光敏電阻讀電壓值
  _read_LightVal();
  
  //從DHT22讀溫度與濕度
  _read_Temp_Hum();
  
  //讀取藍芽通訊命令
  _check_UartCmd();
  
  //定時開關功能，確認時間是否到
  _check_SwOn_Time();
  
  //確認溫度與光亮是否已到
//  _check_SwOn_Condition();

}
