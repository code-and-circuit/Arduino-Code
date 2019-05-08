#include <SoftwareSerial.h>
#include "TFMini.h"

SoftwareSerial lidarSerial(10, 11);

TFMini jazzyLidar;

void setup()
{
  Serial.begin(115200);

  while(!Serial); //Wait for serial

  lidarSerial.begin(TFMINI_BAUDRATE); //Start serial for the lidar at the baudrate for the tfmini

  tfmini.begin(&lidarSerial); //Initalize the lidar via the lidar serial port
}


void loop()
{
  uint16_t distance = tfmini.getDistance(); //Get the distance of the nearest object in centimeters
  uint16_t strength = tfmini.getRecentSignalStrength(); //Get the signal strength

  Serial.print(distance); //Print the distance and strength
  Serial.println(" cm");
  Serial.print("Strength = ");
  Serial.println(strength);

  delay(25); //Wait before reading again
}
