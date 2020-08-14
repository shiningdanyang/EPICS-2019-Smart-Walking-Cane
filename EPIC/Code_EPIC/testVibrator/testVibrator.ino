void setup()
{
	pinMode(10, OUTPUT);
	pinMode(11,OUTPUT);
}

void loop()
{
	digitalWrite(10, HIGH);
	digitalWrite(11, LOW);
	delay(500);
	digitalWrite(11, HIGH);
	digitalWrite(10, LOW);
	delay(500);
}