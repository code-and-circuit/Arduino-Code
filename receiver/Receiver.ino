#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

unsigned long hundredMilliTimer = 0;
unsigned long timeoutTimer = 0;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Listening..."); 
  Wire.begin(5); // join i2c bus
  Wire.onRequest(requestEvent); // register event
  pinMode(13, OUTPUT);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  
  hundredMilliTimer = millis();
  secondTimer = millis();
  timeoutTimer = millis();
}

int counter = 0;
int num = 0;
int timer = 0;
char data[4] = {0,0,0,0};

void loop() 
{
  if (millis() >= secondTimer)
  {
      counter++;
      secondTimer += 1000;
    
   if (counter % 4 == 0)
  {
      Drive(true, 10, 10);
  }
  else if (counter % 4 == 1)
  {
      Drive(true, 0, 0);
  }
  if (counter % 4 == 2)
  {
      Drive(false, 10, 10);
  }
  else if (counter % 4 == 3)
  {
      Drive(true, 0, 0);
  }
    }
  	// every 100 ms
  if (millis() >= hundredMilliTimer)
  {
  	  hundredMilliTimer += 100;
	  
	  Serial.println("Listening..."); 
	  if ( radio.available()) {
	  	
	  	  	digitalWrite(13, HIGH);

	  	Serial.println("Got packet");
	      radio.read(data, 4);
	   
			digitalWrite(13, LOW);

		timeoutTimer = millis();
	  }
  }

  
  if (millis() - timeoutTimer > 1000)
  {
  	Serial.println("Didn't get anything for more than 1 seconds.");
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
  }
}

void EmergencyStop ()
{
  data[0] = 'g';
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
}

void Drive (bool direction, int cm, int speed)
{
  if (direction)
  {
    num = 1;
  }
  else
  {
    num = -1;
  }
  timer++;
  data[3] = speed;
  data[1] = 100 * num; //Set it to forward or backward.
  if (timer >= cm)
  {
    data[1] = 0;
    timer = 0;
  }//Something to set data[1] to 0 after we have reached the 
  //destination a.k.a time.
  
}

void requestEvent() {

	Wire.write((byte*)data, 4);
}

