#include <SoftwareSerial.h>
SoftwareSerial Sim800l(A0, A1); //RX TX
void setup()
{
	
  // put your setup code here, to run once:
  Serial.begin(9600);
  Sim800l.begin(9600);
  delay(10000);
  Sim800l.print("AT+CMGF=1\r\n");
  delay(2000);
  Sim800l.print("AT+CMGS=\"0946412298\"\r\n");
  delay(2000);
  Sim800l.print("bao dong ");
  Sim800l.print("\x1A");
  delay(4000);
  Sim800l.print("ATD0946412298;\r\n");
  delay(4000);
}

void loop()
{
  while (Sim800l.available())
  {
    Serial.write(Sim800l.read());
  }
  while (Serial.available())
  {
    Sim800l.write(Serial.read());
  }
}
