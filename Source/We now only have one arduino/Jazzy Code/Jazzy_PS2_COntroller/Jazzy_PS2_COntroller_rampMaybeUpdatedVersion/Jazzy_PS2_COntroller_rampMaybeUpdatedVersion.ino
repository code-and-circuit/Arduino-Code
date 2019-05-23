#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

Servo leftservo;  // create servo object to control a servo
Servo rightservo; //right motor


PS2X ps2x; // create PS2 Controller Class

//Maybe 0 is full one direction 90 is middle or stopped and 180 could be full other direction
//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;
float leftmotor = 90;
float rightmotor = 90;

float leftmotortarget = 90;
float rightmotortarget = 90;

 float ramp_increment = 5.0f;

void setup()
{
  Serial.begin(57600);
//Vibrate is set to 255 at a good level
  leftservo.attach(5);  // attaches the servo on pin 9 to the servo object
  rightservo.attach(6);  // attaches the servo on pin 9 to the servo object
  rightservo.write(90);
  leftservo.write(90);

  delay(1000);

 //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
 error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
  
}

void loop(){
   /* You must Read Gamepad to get new values
   Read GamePad and set vibration values
   ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
   if you don't enable the rumble, use ps2x.read_gamepad(); with no values
   
   you should call this at least once a second
   */
   
  rightservo.write((int)rightmotor);
  leftservo.write((int)leftmotor);

  Serial.print("R: ");
   Serial.println(rightmotor);
  Serial.print("L: ");
   Serial.println(leftmotor);

 if (rightmotor < rightmotortarget)
    rightmotor += ramp_increment;
 else if (rightmotor > rightmotortarget)
    rightmotor -= ramp_increment;

 if (leftmotor < leftmotortarget)
    leftmotor += ramp_increment;
 else if (leftmotor > leftmotortarget)
    leftmotor -= ramp_increment;
    
 if(error == 1) //skip loop if no controller found
  return; 
  
 if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller 
   
   if(ps2x.ButtonPressed(GREEN_FRET))
     Serial.println("Green Fret Pressed");
   if(ps2x.ButtonPressed(RED_FRET))
     Serial.println("Red Fret Pressed");
   if(ps2x.ButtonPressed(YELLOW_FRET))
     Serial.println("Yellow Fret Pressed");
   if(ps2x.ButtonPressed(BLUE_FRET))
     Serial.println("Blue Fret Pressed");
   if(ps2x.ButtonPressed(ORANGE_FRET))
     Serial.println("Orange Fret Pressed");
     

    if(ps2x.ButtonPressed(STAR_POWER))
     Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
     Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
     Serial.println("DOWN Strum");
  
 
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");

    
    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
        Serial.print("Wammy Bar Position:");
        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
 }

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
         
         
     else if(ps2x.Button(PSB_PAD_UP)) {   
       Forward();
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      }
      else if(ps2x.Button(PSB_PAD_RIGHT)){
        Right();
        VibrateController();
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      }
      else if(ps2x.Button(PSB_PAD_LEFT)){
        Left();
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      }
      else if(ps2x.Button(PSB_PAD_DOWN)){
       Back();
       Serial.print("DOWN held this hard: ");
       
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      }   
      else
      {
          Stop();
      }
  
    
     // vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
     // Serial.println(ps2x.Analog(PSAB_BLUE));
     // Serial.println(PSAB_BLUE);
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
     
       
         
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         
    if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
    
    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");    
    
    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
    } 
    
    
 }
 
 
 delay(20);
     
}

void Forward ()
{
  ramp_increment = 5.0f;
   rightmotortarget = 0;
   leftmotortarget = 0;
}

void Right ()
{
  ramp_increment = 5.0f;
   rightmotortarget = 180;
   leftmotortarget = 0;
}

void Left ()
{
  ramp_increment = 5.0f;
    rightmotortarget = 0;
    leftmotortarget = 180;
}

void Back ()
{
  ramp_increment = 5.0f;
     rightmotortarget = 180;
     leftmotortarget = 180;
}

void Stop()
{
  ramp_increment = 20.0f;
    rightmotortarget = 90;
    leftmotortarget = 90;
}

void VibrateController ()
{
     vibrate = 255;
}








