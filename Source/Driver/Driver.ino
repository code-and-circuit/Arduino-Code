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

void startupChair();

void setup()
{
  Wire.begin(0x33);
  Wire.onReceive(receiveEvent);

  pinMode(13, OUTPUT);

  pinMode(10, OUTPUT);
  digitalWrite (10, LOW);  // start serial line LOW

  Serial.begin(115200);
  delay(1000);

  startupChair();

  tenMilliTimer = millis();
  oneSecondTimer = millis();
  hundredMilliTimer = millis();
}

int coms[3] = {0, 0, 0};
union foo
{
  byte a;
  char b;
};

int data[3] = {0, 0, 0};
int dataSet = 0;

void receiveEvent(int dummy)
{

  Serial.print(millis());
  Serial.println(" receiveData");
  while (Wire.available())
  {
    Wire.read();
    data[dataSet] = Wire.read();
    Wire.read();
    ++dataSet;

    if (dataSet == 4)
    {

      if (data[0] >= -100 && data[0] <= 100 && data[1] >= -100 && data[1] <= 100 && data[2] >= 0 && data[2] <= 0xe)
      {

        //	Serial.println("Received command: ");

        Serial.print("Forward/Backward: ");
        Serial.print (data[0]);

        Serial.print(" Left/write: ");
        Serial.print(data[1]);

        Serial.print(" Speed: ");
        Serial.println(data[2]);

        curFor = data[0];
        curTurn = data[1];
        speed = data[2];
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
  /*	Serial.print(millis());
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
