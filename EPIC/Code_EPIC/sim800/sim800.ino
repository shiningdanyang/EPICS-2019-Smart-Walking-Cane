#define buttonSim 23
void setup()
{
	Serial.begin(9600);
	pinMode(buttonSim, INPUT_PULLUP);
}

void loop()
{
	if(digitalRead(buttonSim) == LOW)
	{
		Serial.print("AT+CMGF=1\r\n");
		delay(2000);
		Serial.print("AT+CMGS=\"01659795596\"\r\n");
		delay(2000);
		Serial.print("bao dong: nguoi su dung vua bi nga ");
    	Serial.print("\x1A");
    	delay(4000);
		Serial.print("ATD01659795596;\r\n");
    	delay(4000);
	}
}