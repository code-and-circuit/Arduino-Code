#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

unsigned long hundredMilliTimer = 0;

// or 2 and 3 depending how remote is wired
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
void setup() {
	pinMode(13, OUTPUT);
	Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_HIGH);
  
  hundredMilliTimer = millis();
}

void loop(){

  
  	// every 100 ms
  if (millis() >= hundredMilliTimer)
  {
  digitalWrite(13, HIGH);
  hundredMilliTimer += 100;
	  
  char data[4] = {'g', 0, 0, 0xa};

  int potValueX = analogRead(A0);
  data[1] = map(potValueX, 0, 1023, -100, 100);
  
  int potValueY = analogRead(A1);
  data[2] = map(potValueY, 0, 1023, -100, 100);
  
  int potValueSpeed = analogRead(A2);
  data[3] = 0xd; //map(potValueSpeed, 0, 1023, 0, 0xe);
  
  radio.write(data, 4);
  digitalWrite(13, LOW);

//  Serial.println("Finished sending");
	}

  } 
