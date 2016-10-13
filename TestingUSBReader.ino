#define POWER 2
#define BACK  3
#define PLAY  4
#define NEXT  5
#define PLAYER_ON 6
#define USB_PIN 7
#define BUTTON_AUX 8

#define LED 13

#define STEERING_KEYS_A A0
#define STEERING_KEYS_B A1
#define LINE_PIN 9

#define PUSH_TIME 100
#define LONG_PUSH_TIME 1000
#define MAIN_LOOP_TIME 200
#define SERIAL_DEBUG 

bool playerState; 
bool fistTimeOn;  
int playButtonCount=0;

void powerPress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the POWER hold button pressed.");
#endif
          
        pinMode(POWER, OUTPUT);          
        digitalWrite(POWER, LOW);
        delay(LONG_PUSH_TIME);
        pinMode(POWER, INPUT);
        digitalWrite(POWER, HIGH);
}

void modePress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the MODE button pressed.");
#endif
       
        pinMode(POWER, OUTPUT);          
        digitalWrite(POWER, LOW);
        delay(PUSH_TIME);
        pinMode(POWER, INPUT);       
}
 
void backPress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the BACK button pressed.");
#endif 
        pinMode(BACK, OUTPUT);          
        digitalWrite(BACK, LOW);
        delay(PUSH_TIME);
        pinMode(BACK, INPUT);     
 }   
 
void playPress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the PLAY button pressed.");
#endif
        pinMode(PLAY, OUTPUT);          
        digitalWrite(PLAY, LOW);
        delay(PUSH_TIME);
        pinMode(PLAY, INPUT);     
}    
          
void nextPress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the NEXT button pressed.");
#endif
        pinMode(NEXT, OUTPUT);          
        digitalWrite(NEXT, LOW);
        delay(PUSH_TIME);
        pinMode(NEXT, INPUT); 
}

void auxPress()
{
#ifdef SERIAL_DEBUG
        Serial.println("Emulating the AUX button pressed.");
#endif
        pinMode(BUTTON_AUX, OUTPUT);          
        digitalWrite(BUTTON_AUX, LOW);
        delay(PUSH_TIME*4);
        pinMode(BUTTON_AUX, INPUT);     
}

void lineInSet(bool state)
{
#ifdef SERIAL_DEBUG
        Serial.print("Emulating the AUX LINE IN plugged -> ");
        Serial.println(state?"True":"False");
#endif  
    if(state)
    {
          pinMode(LINE_PIN, OUTPUT);          
          digitalWrite(LINE_PIN, LOW);
          delay(PUSH_TIME);
    } 
    else
    {
          pinMode(LINE_PIN, INPUT);     
    }
}

void checkSteeringKeys()
{
        //Audio player control
        int keyA = analogRead(STEERING_KEYS_A)/2;
        int keyB = analogRead(STEERING_KEYS_B)/2;
                
/*
#ifdef SERIAL_DEBUG
        Serial.print("Reading remote control: KEYA=");
        Serial.print(keyA);
        Serial.print(", KEYB="); 
        Serial.println(keyB);
#endif 
*/     
        if(keyA>105 & keyA<125)  //SEEK UP for 1.7V +-10%    6(P)
        {
          nextPress();
        }
        else if(keyB>105 & keyB<125)  //SEEK DOWN for 1.7V +-10%  16(L)
        {
          backPress();
        }
        else if((keyA>=0 & keyA<20) & (keyB>=0 & keyB<20)) //Both, SEND & END for 0V 
        {
          playButtonCount++; // Count time while buttons are pushed.
        }
        else
        {
          if(playButtonCount)  //If the buttons are released and play was pressed
          {
            if(playButtonCount<5) //Emulate Play press if is a short press
            {
              playPress();
            }
            else                //Emulate Mode press if is a long hold press.
            {
              modePress();
            }
          }
          playButtonCount=0;
        }
   
}

void powerOnPlayer()
{   
  while(!playerState)
  {
#ifdef SERIAL_DEBUG    
    Serial.print(playerState?"-ON-":"-OFF-");
    Serial.print("--POWERING ON--");
#endif    

    powerPress();    
    playerState=digitalRead(PLAYER_ON);

#ifdef SERIAL_DEBUG  
    Serial.println(playerState?"-ON-":"-OFF-");
#endif

    if(playerState) 
    {
        blinkLed(400,10); //Wait to loading procedure
        if(!fistTimeOn)
        {
#ifdef SERIAL_DEBUG  
            Serial.println("-USB MODE?-");
#endif            
            modePress(); //Radio mode skip
            blinkLed(500,1); //Wait to loading procedure
            modePress(); //AUX mode skip
        }
        fistTimeOn=false;
        lineInSet(HIGH);
        blinkLed(200,3); //Wait to radio detection of line-in
        auxPress(); 
    }  
  }
}

void powerOffPlayer()
{
  while(playerState)
  {  
    
#ifdef SERIAL_DEBUG  
    Serial.print(playerState?"-ON-":"-OFF-");
    Serial.print("--POWERING OFF--"); 
#endif
    
    lineInSet(LOW); 
    delay(1000); //Wait cause the USB unplugged
    powerPress();
    delay(500); //Wait to chip standby
    playerState=digitalRead(PLAYER_ON);
    
#ifdef SERIAL_DEBUG      
    Serial.println(playerState?"-ON-":"-OFF-");
#endif    

  }  
}


void setup() {
#ifdef SERIAL_DEBUG  
    Serial.begin(9600);    
#endif    
    pinMode(POWER, INPUT);    //For emulate PLAYER buttons pressing                 
    pinMode(BACK, INPUT);     //INPUT MODE is set to avoid electrical isues       
    pinMode(PLAY, INPUT);          
    pinMode(NEXT, INPUT);          
  
    pinMode(USB_PIN, INPUT);      //For USB detection
    digitalWrite(USB_PIN, HIGH);  //Set pullup
    
    pinMode(PLAYER_ON, INPUT);
    
    pinMode(BUTTON_AUX, INPUT); //For emulate the AUX button press
    pinMode(LINE_PIN, INPUT);   //For emulate the LINE IN plugged
    
    pinMode(LED, OUTPUT);          
    digitalWrite(LED, HIGH); 
  
    playerState=digitalRead(PLAYER_ON);
    fistTimeOn = true;
    
}

void blinkLed(int t, int x)
{
  for(int i=0; i<x;i++)
  {
    digitalWrite(LED,LOW);
    delay(t/2);
    digitalWrite(LED,HIGH);
    delay(t/2);    
  }
}

void loop() {
 
    playerState=digitalRead(PLAYER_ON);
    digitalWrite(LED,playerState);
/*    
    
    if(!digitalRead(USB_PIN))   //Ever that USB is connected
    {
        delay(MAIN_LOOP_TIME);
        if(!digitalRead(USB_PIN))   //Confirmation
        {   
            checkSteeringKeys() ;
            //If required, power on the player 
            powerOnPlayer();
        }
    }
    else
    {
        //Power off the player  
        delay(MAIN_LOOP_TIME);
        if(digitalRead(USB_PIN))   //Confirmation
        {
            powerOffPlayer();
        }
    }
*/
}


#ifdef SERIAL_DEBUG
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    switch(inChar)
    {
      case 'X':
        powerPress();       
        break;
      case 'x':
        modePress();       
        break;
      case 'b':
        backPress();     
        break;   
      case 'p':
        playPress();     
        break;                  
      case 'n':
        nextPress(); 
        break;
      case 'a':
        auxPress(); 
        break;   
      case 'L':
        lineInSet(HIGH); 
        break;  
      case 'l':
        lineInSet(LOW); 
        break;  
      case 'S':
        powerOnPlayer(); 
        break;
      case 's':
        powerOffPlayer(); 
        break;                          
      default:
        Serial.println("--No valid command--");
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
      break;  
    }
  }
}
#endif



