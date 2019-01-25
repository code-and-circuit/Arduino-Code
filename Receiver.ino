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
  timeoutTimer = millis();
}

char data[4] = {0,0,0,0};

void loop() 
{
  
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

void requestEvent() {

	Wire.write((byte*)data, 4);
}

