#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

unsigned long secondTimer = 0;
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

int timer2 = 0;
int turntime = 0;
int counter = 0;
int time = 0;
int num = 0;
int timer = 0;
char data[4] = {'g',0,0,0};

void loop() 
{
  if (millis() >= secondTimer)
  {
      counter++;
      secondTimer += 1000;
    
   if (counter % 4 == 0)
  {
      Drive(true, 10);
  }
  /*
  else if (counter % 4 == 1)
  {
      Drive(true, 0);
  }
  if (counter % 4 == 2)
  {
      Drive(false, 10);
  }
  else if (counter % 4 == 3)
  {
      Drive(true, 0);
  }*/
    }
  	// every 100 ms
  if (millis() >= hundredMilliTimer)
  {
  	  hundredMilliTimer += 100;
	  /*
	  Serial.println("Listening..."); 
	  if ( radio.available()) {
	  	
	  	  	digitalWrite(13, HIGH);

	  	Serial.println("Got packet");
	      radio.read(data, 4);
	   
			digitalWrite(13, LOW);
	  }
	  */
        timer++;
        timer2++;
      if (timer >= time)
      {
    	 data[1] = 0;
      //timer = 0;
      }
      if (timer2 >= turntime)
      {
    	  data[2] = 0;
      //timer2 = 0;
      }
      //Something to set data[1] to 0 after we have reached the 
  //destination a.k.a time.
     Serial.println("Forwards data: ");
     Serial.println(data[1], DEC);
     Serial.println("Turning data: "); 
     Serial.println(data[2], DEC);
     Serial.println("Timer one: "); 
     Serial.println(timer);
     Serial.println("Timer two for turning: ");
     Serial.println(timer2);
     Serial.println("Run time forwards: ");
     Serial.println(time);
     Serial.println("Run time turning: ");
     Serial.println(turntime); 

	timeoutTimer = millis();
	 
  }

  /*
  if (millis() - timeoutTimer > 1000)
  {
  	Serial.println("Didn't get anything for more than 1 seconds.");
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
  }
  */
}

void EmergencyStop ()
{
  data[0] = 'g';
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  timer = 100000;
  timer2 = 1000000;
}

void Drive (bool direction, int cm)
{
  if (direction)
  {
    num = 1;
  }
  else
  {
    num = -1;
  }
  //We'll need a bool to check if we are already moving so that we don't attempt to initiate multiple
    timer = 0;
  data[3] = 13;
  data[1] = 100 * num; //Set it to forward or backward.
  time = cm * 1;//Change this to a time...
  
}

void Turn (bool direction, int degrees)
{
  if (direction)
  {
    num = 1;
  }
  else
  {
    num = -1;
  }
  timer2 = 0;
  data[3] = 13;
  data[2] = 100 * num; //Set it to right or left.
  turntime = degrees * 1;//Change this to a time...
  
}

void requestEvent() {

	Wire.write((byte*)data, 4);
}
