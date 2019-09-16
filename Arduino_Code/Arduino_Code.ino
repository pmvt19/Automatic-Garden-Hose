#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

File myFile;
int pinCS = 10;
int temperaturePin = 0;
const int soilMoisturePin = A1;
float soilMoisture = 0;
float soilMoistureRaw = 0;
int todayDate;
bool wateredToday;
int solenoidPin = 1;
int manualWater = 0;
int manualWaterPin = 2;
int wateringTime;
bool rainToday;
int rainTodayPin = 3;
int moistureThreshold = 58;
int Chipset = 10;

void setup() 
{
  // put your setup code here, to run once:
  rtc.begin();
  pinMode(pinCS,OUTPUT);
  pinMode(soilMoisturePin, INPUT);
  pinMode(manualWaterPin, INPUT);
  pinMode(rainTodayPin, INPUT);
  Serial.begin(9600);

  if(SD.begin(Chipset))
  {
    Serial.println("SD card is ready to use");
  }
  else
  {
    Serial.println("SD card initialization failed");
  }
  wateredToday = false;
  wateringTime = 10000;
  rainToday = false;
}

void loop() 
{
  
  float voltage, degreesC, degreesF;
  DateTime now = rtc.now();
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  myFile = SD.open("text.txt", FILE_WRITE);
  
  if(myFile)
  {
    myFile.print(now.hour());
    myFile.print(":");
    myFile.print(now.minute());
    myFile.print(":");
    myFile.print(now.second());
    myFile.print(",");
    myFile.print(degreesF);
    myFile.print(",");
    myFile.println(getSoilMoisture());
    myFile.close();
  }
  Serial.print(digitalRead(rainTodayPin));
  Serial.print(digitalRead(manualWaterPin));
  Serial.print(myFile);
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.print(",");
  Serial.print(degreesF);
  Serial.print(",");
  Serial.println(getSoilMoisture());
  
  delay(1000);

  if(now.hour()== 0)
  {
    wateredToday = false;
    rainToday = false;
  }

  if(digitalRead(manualWaterPin) == HIGH)
  {
    manualWater = HIGH;
  }
  else
  {
    manualWater = LOW;
  }

  if(digitalRead(rainTodayPin) == HIGH)
  {
    rainToday = true;
  }
  if(((wateredToday == false) && (now.hour()>17) && (now.hour()<19) && (getSoilMoisture() < moistureThreshold)) || (manualWater == HIGH))
  {
    digitalWrite(solenoidPin, HIGH);
    if(rainToday)
    {
      delay(1);
    }
    else
    {
      delay(wateringTime);
    }
    
    digitalWrite(solenoidPin, LOW);
  }
  wateredToday = true;
}

float getVoltage(int pin)
{
  
  return (analogRead(pin) * 0.004882814);
  

}

float getSoilMoisture()
{
  soilMoistureRaw = getVoltage(soilMoisturePin);
  
  if (soilMoistureRaw < 1.1) 
  {
    soilMoisture = (10 * soilMoistureRaw) - 1;
  }
  else if (soilMoistureRaw < 1.3) 
  {
    soilMoisture = (25 * soilMoistureRaw) - 17.5;
  }
  else if (soilMoistureRaw < 1.82) 
  {
    soilMoisture = (48.08 * soilMoistureRaw) - 47.5;
  }
  else if (soilMoistureRaw < 2.2) 
  {
    soilMoisture = (26.32 * soilMoistureRaw) - 7.89;
  }
  else 
  {
    soilMoisture = (62.5 * soilMoistureRaw) - 87.5;
  }
  return soilMoisture;
}


