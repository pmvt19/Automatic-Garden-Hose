
const int soilMoisturePin = A1;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(getVoltage(soilMoisturePin));
delay(1000);
}

float getVoltage(int pin)
{
  
  return (analogRead(pin) * 0.004882814);
  

}
