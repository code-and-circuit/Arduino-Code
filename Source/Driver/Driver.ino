// This code runs on the arduino connected to the chair. Pin 10 is connected to the white wire.
// SCL/SDA Connected to controller arduino.

#include "SoftwareSerialParityHalfDuplex.h"
#include <Wire.h>

SoftwareSerialParityHalfDuplex mySerial(10, 10);

unsigned long tenMilliTimer = 0;

unsigned long hundredMilliTimer = 0;

// this sketch rocks the chair forward and backwards just for testing

unsigned long oneSecondTimer = 0;

int curFor = 0;
int curTurn = 0;
int speed = 0x0c;

void setup()
{
  Wire.begin(10);
  Wire.onReceive(receiveEvent);

  pinMode(13, OUTPUT);

  pinMode(10,OUTPUT);
  digitalWrite (10, LOW);  // start serial line LOW

  Serial.begin(115200);
  delay(1000);

  startupChair();

  tenMilliTimer = millis();
  oneSecondTimer = millis();
  hundredMilliTimer = millis();
}

int coms[3] = {0,0,0};
union foo
{
  byte a;
  char b;
};


void receiveEvent()
{
  Serial.print(millis());
  Serial.println(" receiveData");
  int comsN = -1;
  while (Wire.available())
  {
    Serial.println("Got data");
    if (comsN == -1)
    {
      union foo bar;
      bar.b = Wire.read(); // receive byte as a character
      int c = bar.b;
      // Serial.println(c);
      if (c == 'g')
      {
        comsN++;
      }
    }
    else if (comsN >= 0)
    {
      union foo bar;
      bar.a = Wire.read(); // receive byte as a character
      //      Serial.println(c);
      int c = bar.b;
      coms[comsN] = c; // receive byte
      comsN++;

      if (comsN == 3)
      {
        comsN = -1;
        int a = coms[0];
        int t = coms[1];
        int s = coms[2];
        /*
        Serial.print(a);
        Serial.print(" ");
        Serial.print(t);
        Serial.print(" ");
        Serial.println(s);
        */
        if (a >= -100 && a <= 100 && t >= -100 && t <= 100 && s >= 0 && s <= 0xe)
        {

          //  Serial.println("Received command: ");

          Serial.print("Forward/Backward: ");
          Serial.print (a);

          Serial.print(" Left/write: ");
          Serial.print(t);

          Serial.print(" Speed: ");
          Serial.println(s);

          curFor = a;
          curTurn = t;
          speed = s;
        }
        else
        {
          Serial.println("Received bad command");
          curFor = 0;
          curTurn = 0;
          speed = 0;
        }
      }
    }
  }
}

void startupChair()
{
  // activate base with 110 ms pulse
  digitalWrite (10, HIGH);
  delay(110);

  // start up software serial with even parity
  mySerial.begin(38400, EVEN);

  Serial.println("Started up base");

  // send init packet
  unsigned char buf2[2] = { 0x52, 0xad };
  mySerial.write(buf2, 2);

  // base will respond, but we don't care about the response
  delay(10);
}

int cycleCount = 0;

void sendJoystickPacket(int forward, int turn, int speed)
{
  unsigned char buffer[6] = { 0x4a, 0x01, 0x03, 0x00, 0x00, 0x00 };

  buffer[2] = speed;
  buffer[3] = forward;
  buffer[4] = turn;
  /*  Serial.print(millis());
    Serial.print(" Sending :");
    Serial.print(forward);
    Serial.print(" ");
    Serial.print(turn);
    Serial.print(" ");
    Serial.println(speed);
  */
  buffer[5] = 0xff - (buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4]);

  mySerial.write(buffer, 6);
}

void loop()
{
  unsigned long milli = millis();

  // once per second
  if (milli >= oneSecondTimer)
  {


    cycleCount++;
    oneSecondTimer += 1000;

  }
  if (milli >= tenMilliTimer)
  {
    tenMilliTimer += 10;

    if (cycleCount <= 1)
    {
      // don't drive for first second, otherwise it will not work
      sendJoystickPacket(0, 0, speed);
    }
    else
    {
      sendJoystickPacket(curFor, curTurn, speed);
    }
  }
  // chair will respond, but we don't care
}
