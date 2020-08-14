#include <NewPing.h>
///////////////////////////////////////////////////////
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
// ////////////////////////////////////////////////////
#include <SoftwareSerial.h>
// /////////////////////////////////////////////
SoftwareSerial Sim800l(A0, A1); // RX TX
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_LEFT 3 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_LEFT 2 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_LEFT 10 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_RIGHT 5
#define ECHO_PIN_RIGHT 4
#define MAX_DISTANCE_RIGHT 10
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_FRONT 7
#define ECHO_PIN_FRONT 6
#define MAX_DISTANCE_FRONT 10
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_BOTTOM 12
#define ECHO_PIN_BOTTOM 11
#define MAX_DISTANCE_BOTTOM 10
// /////////////////////////////////////////////////////
NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE_LEFT); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE_RIGHT); // NewPing setup of pins and maximum distance.
NewPing sonarFront(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_FRONT); // NewPing setup of pins and maximum distance.
NewPing sonarBottom(TRIGGER_PIN_BOTTOM, ECHO_PIN_BOTTOM, MAX_DISTANCE_BOTTOM); // NewPing setup of pins and maximum distance.
// /////////////////////////////////////////////////////
// #define SENSOR_PIN A2
#define BUZZER_PIN 13
// /////////////////////////////////////////////////////
#define MOTOR_LEFT 9
#define MOTOR_RIGHT 10
#define power 200
// /////////////////////////////////////////////////////
int getDistanceLeft()
{
  return sonarLeft.ping_cm();
}

int getDistanceRight()
{
  return sonarRight.ping_cm();
}

int getDistanceFront()
{
  return sonarFront.ping_cm();
}

int getDistanceBottom()
{
  return sonarBottom.ping_cm();
}

void warningLeft()
{
  analogWrite(MOTOR_LEFT, power); // 100
}

void notWarningLeft()
{
  digitalWrite(MOTOR_LEFT, LOW);
}

void warningRight()
{
  analogWrite(MOTOR_RIGHT, power);
}

void notWarningRight()
{
  digitalWrite(MOTOR_RIGHT, LOW);
}

void warningFront()
{
  digitalWrite(MOTOR_RIGHT, power/2);
  digitalWrite(MOTOR_LEFT, power/2);
}

void turnOff()
{
  digitalWrite(MOTOR_RIGHT, LOW);
  digitalWrite(MOTOR_LEFT, LOW);
}

void warningAll()
{
  digitalWrite(MOTOR_LEFT, power);
  digitalWrite(MOTOR_RIGHT, power);
}

void setup()
{
  delay(10000);
  // pinMode(1,OUTPUT);
  // digitalWrite(1, HIGH);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  Sim800l.begin(9600);
  // delay(20000);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(1, LOW);
}

void loop()
{
  delay(50);
  mpu6050.update();
  int angleX = mpu6050.getAngleX();
  int angleY = mpu6050.getAngleY();

  if (abs(angleX)>70||abs(angleY)>70)
  {
    // Serial.begin(9600);
    // Sim800l.begin(9600);
    // delay(10000);
    Sim800l.print("AT+CMGF=1\r\n");
    delay(2000);
    Sim800l.print("AT+CMGS=\"0946412298\"\r\n");
    delay(2000);
    Sim800l.print("bao dong ");
    Sim800l.print("\x1A");
    delay(4000);
    Sim800l.print("ATD0946412298;\r\n");
    delay(4000);
    while (1)
    {
      // digitalWrite(13,LOW);
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(1, HIGH);
    }
  }
  int distanceLeft = getDistanceLeft();
  int distanceRight = getDistanceRight();
  int distanceFront = getDistanceFront();
  int distanceBottom = getDistanceBottom();
  Serial.print(" left");
  Serial.print(distanceLeft);
  Serial.print(" right");
  Serial.print(distanceRight);
  Serial.print(" front");
  Serial.println(distanceFront);
  if ((distanceLeft != 0) && (distanceRight != 0))
  // nếu như 2 bên
  {
    warningLeft();
    notWarningRight();
    delay(1000);
    warningRight();
    notWarningLeft();
    delay(1000);
    return;
  }
  if ((distanceLeft != 0) && (distanceRight == 0))
  {
    warningLeft();
    notWarningRight();
    return;
  }
  else
    if ((distanceLeft == 0) && (distanceRight != 0))
    {
      warningRight();
      notWarningLeft();
      return;
    }
  else
  // if ((distanceFront != 0) || (distanceBottom))
  if (distanceFront != 0)
  {
    warningFront();
    return;
  }
  else
    turnOff();
}
