#include "NewPing.h"
#include "MPU6050_tockn.h"
#include <Wire.h>
MPU6050 mpu6050(Wire);
#include "TinyGPS.h"
// ////////////////////////////////////////////////////
#include <SoftwareSerial.h>
// /////////////////////////////////////////////
SoftwareSerial Sim800l(A0, A1); // RX TX
TinyGPS gps;
SoftwareSerial ss(A2, A3);
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_LEFT 2
#define ECHO_PIN_LEFT 3
#define MAX_DISTANCE_LEFT 100
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_RIGHT 4
#define ECHO_PIN_RIGHT 5
#define MAX_DISTANCE_RIGHT 100
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_FRONT 7
#define ECHO_PIN_FRONT 6
#define MAX_DISTANCE_FRONT 100
// /////////////////////////////////////////////////////
#define TRIGGER_PIN_BOTTOM 8
#define ECHO_PIN_BOTTOM 9
#define MAX_DISTANCE_BOTTOM 100
// /////////////////////////////////////////////////////
NewPing sonarLeft(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE_LEFT); // NewPing setup of pins and maximum distance.
NewPing sonarRight(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE_RIGHT); // NewPing setup of pins and maximum distance.
NewPing sonarFront(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE_FRONT); // NewPing setup of pins and maximum distance.
NewPing sonarBottom(TRIGGER_PIN_BOTTOM, ECHO_PIN_BOTTOM, MAX_DISTANCE_BOTTOM); // NewPing setup of pins and maximum distance.
// /////////////////////////////////////////////////////
// #define SENSOR_PIN A2
#define BUZZER_PIN 13
// /////////////////////////////////////////////////////
#define MOTOR_LEFT 11//8
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
  digitalWrite(MOTOR_RIGHT, power / 3);
  digitalWrite(MOTOR_LEFT, power / 3);
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
  Wire.begin();
  ss.begin(4800);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  Sim800l.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop()
{
  delay(50);
  mpu6050.update();
  int angleX = mpu6050.getAngleX();
  int angleY = mpu6050.getAngleY();

  if (abs(angleX) > 85 || abs(angleY) > 85)
  {
    delay(1000);
    mpu6050.update();
    angleX = mpu6050.getAngleX();
    angleY = mpu6050.getAngleY();

    if (abs(angleX) > 85 || abs(angleY) > 85)
    {
      // Serial.begin(9600);
      // Sim800l.begin(9600);
      // delay(10000);

      //đọc toạ độ của module gps/////////////////////////////////////////////////////////
      bool newData = false;
      unsigned long chars;
      unsigned short sentences, failed;
      unsigned long startGetGPS = millis();
      while (millis() - startGetGPS < 5000)
      {
        for (unsigned long start = millis(); millis() - start < 1000;)
        {
          while (ss.available())
          {
            char c = ss.read();
            // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
            if (gps.encode(c)) // Did a new valid sentence come in?
              newData = true;
          }
        }

        if (newData)
        {
          float flat, flon;
          unsigned long age;
          gps.f_get_position(&flat, &flon, &age);
          //        Serial.print("LAT=");
          //        Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
          //        Serial.print(" LON=");
          //        Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
          //        Serial.print(" SAT=");
          //        Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
          //        Serial.print(" PREC=");
          //        Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
        }

        gps.stats(&chars, &sentences, &failed);
        //      Serial.print(" CHARS=");
        //      Serial.print(chars);
        //      Serial.print(" SENTENCES=");
        //      Serial.print(sentences);
        //      Serial.print(" CSUM ERR=");
        //      Serial.println(failed);
        if (chars == 0)
          ;
        //        Serial.println("** No characters received from GPS: check wiring **");
        /////////////////////////////////////////////////////////////////////////////////////////////////
      }

      Sim800l.print("AT+CMGF=1\r\n");
      delay(2000);
      Sim800l.print("AT+CMGS=\"0946412298\"\r\n");
      delay(2000);
      Sim800l.print("warning at location: ");
      //cấu trúc link googlemap https://www.google.com/maps/@16.0733143,108.1491092,16.8z
      String location = String("https://www.google.com/maps/") + String("@") + String(flat) + String(flon) + String("z");
      Sim800l.print("\x1A");
      delay(4000);
      Sim800l.print("ATD0946412298;\r\n");
      delay(4000);


      while (1)
      {
        // digitalWrite(13,LOW);
        digitalWrite(BUZZER_PIN, HIGH);
        // digitalWrite(1, HIGH);
      }
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
  //  if ((distanceLeft != 0) && (distanceRight != 0))
  //    // nếu như 2 bên
  //  {
  //    warningLeft();
  //    notWarningRight();
  //    delay(1000);
  //    warningRight();
  //    notWarningLeft();
  //    delay(1000);
  //    return;
  //  }
  if ((distanceLeft != 0) && (distanceRight == 0))
  {
    warningLeft();
    notWarningRight();
    return;
  }
  else if ((distanceLeft == 0) && (distanceRight != 0))
  {
    warningRight();
    notWarningLeft();
    return;
  }
  else if ((distanceFront != 0) || (distanceBottom != 0))
    // if (distanceFront != 0)
  {
    warningFront();
    return;
  }
  else
    turnOff();
}
