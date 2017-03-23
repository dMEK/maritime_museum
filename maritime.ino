 //Sketch for MacGyver
//Maritime Museum interactive Project
//Dillon MacEwan
//12 switch pins are digital pins 22 - 33
//squid lights pins 34 - 36 squid wav pin 37
//engine power pins 38 - 42 engine chaser pins 43 -45
//timer light pins 46 - 50
//light pin 51 vent pin 52
//Start button pin 2 strt led pin 53
// noise triggers are pins 3 - 9


//define inputs here including pins for switchboard array, "on button"



#define START 2
//define noise pins
#define ENGINE_NOISE 3
#define I_CAN_SEE 4
#define FRESH_AIR 5
#define FULL_SHEILD 6
#define OVERLOAD 7
#define MOVING_NOW 8
#define FULL_SPEED 9

//define plug switches
#define LIGHTPLUG 22
#define VENTPLUG1 23
#define VENTPLUG2 24
#define SQUIDPLUG1 25
#define SQUIDPLUG2 26
#define SQUIDPLUG3 27
#define SQUIDPLUG4 28
#define ENGINEPLUG1 29 
#define ENGINEPLUG2 30 
#define ENGINEPLUG3 31 
#define ENGINEPLUG4 32
#define ENGINEPLUG5 33

//define all output pins
#define SQUIDLIGHT1 34
#define SQUIDLIGHT2 35
#define SQUIDLIGHT3 36


#define ENGINELIGHT1 38
#define ENGINELIGHT2 39
#define ENGINELIGHT3 40
#define ENGINELIGHT4 41
#define ENGINELIGHT5 42
#define ENGINECHASER1 43
#define ENGINECHASER2 44
#define ENGINECHASER3 45

#define TIMER1 46
#define TIMER2 47
#define TIMER3 48
#define TIMER4 49
#define TIMER5 50

#define LIGHT 51
#define VENT 52
#define START_LED 53

int ventMode = 0; //vent mode status
int squidMode = 0; //squid mode status
int engineMode = 0; //engine mode status
int start = 0;  //run status
int squidAlert = HIGH;
int startFlash = HIGH;
int fanLED = ENGINECHASER1;

int lightSound = 0;
int fanSound = 0;
int squidSound = 0;
int movingSound = 0;
int fullSpeedSound = 0;


//set counter variables
unsigned long timer;
unsigned long timerStart;
unsigned long fanTime;
unsigned long alertTime;
unsigned long chargeTime;
unsigned long startTime;



int switched = 0;
int fanDelay = 500;
int alertDelay = 500;
int startDelay = 500;

void setup()
{
  //initialise all pins
  //initialise LED pins
  
  //intitialise button pins
  pinMode(START,  INPUT_PULLUP);
  for (int i = LIGHTPLUG; i<= ENGINEPLUG5; i++)
  {
    pinMode(i,  INPUT_PULLUP);
  }
    
  //initialise output pins 
  for (int i = SQUIDLIGHT1; i <= START_LED; i++)
  {
    pinMode(i, OUTPUT);
  }
       startTime = millis();
         while (start == 0)//wait for start button to be pushed
  {
    if ((millis() - startTime)>=startDelay)
    {
      startFlash = !startFlash;
     
      startTime = millis();
      delay(40);
    }
    digitalWrite (START_LED, startFlash);
    
    int pushStart = digitalRead (START);
    if (pushStart == LOW)
    start = 1;
    
    delay(50);  //debounce
    
    digitalWrite (START_LED, LOW);
  
  }
  //powerup!
  for (int i = TIMER1; i <= TIMER5; i++)
    {
      digitalWrite (3, HIGH);
      digitalWrite (i, HIGH);
      delay (2000);
    }
    digitalWrite (3, LOW);
    while (start == 1)//wait for plug to be triggered
  {
    for (int i = LIGHTPLUG; i< ENGINEPLUG5; i++)
    {
      if (digitalRead(i) == 0)
        start = 2;
    }
    
  
  }
  timerStart = millis();

}

void loop()
{
  timer = millis() - timerStart;
  
  //read plug switches and set modes
  if (digitalRead (LIGHTPLUG) == LOW)
  {
    digitalWrite (LIGHT, HIGH);
    if (lightSound == 0)
    {
      digitalWrite (I_CAN_SEE, HIGH);
      delay (30);
      digitalWrite (I_CAN_SEE, LOW);
      lightSound = 1;
    }
  }
  else
  {
    digitalWrite (LIGHT, LOW);
    if (lightSound == 1)
    {
    lightSound = 0;
    delay (30);
    }
  }
    
    if ( (digitalRead(VENTPLUG1) == LOW)&&(digitalRead(VENTPLUG2) == LOW))
    {
    digitalWrite (VENT, HIGH);
    if (fanSound == 0)
    {
      digitalWrite (FRESH_AIR, HIGH);
      delay (30);
      digitalWrite (FRESH_AIR, LOW);
      fanSound = 1;
    }
    }
  else
    {
    digitalWrite (VENT, LOW);
    if (fanSound == 1)
    {
      fanSound = 0;
      delay (30);
    }
    }
  
  switched = 0;
  for (int i = SQUIDPLUG1; i<= SQUIDPLUG4; i++)
    {
      if (digitalRead(i) == 0)
        switched++;
    }
  squidMode = switched;
  
  switched = 0;
  for (int i = ENGINEPLUG1; i<= ENGINEPLUG5; i++)
    {
      if (digitalRead(i) == 0)
        switched++;
    }
  engineMode = switched;
  
  //set mode behaviours
  switch (squidMode)
  {
    case 0:
    digitalWrite (SQUIDLIGHT1, LOW);
    digitalWrite (SQUIDLIGHT2, LOW);
    digitalWrite (SQUIDLIGHT3, LOW);
    digitalWrite (OVERLOAD, LOW);
    squidSound = 0;
    break;
    case 1:
    digitalWrite (SQUIDLIGHT1, HIGH);
    digitalWrite (SQUIDLIGHT2, LOW);
    digitalWrite (SQUIDLIGHT3, LOW);
    digitalWrite (OVERLOAD, LOW);
    squidSound = 0;
    break;
    case 2:
    digitalWrite (SQUIDLIGHT1, HIGH);
    digitalWrite (SQUIDLIGHT2, HIGH);
    digitalWrite (SQUIDLIGHT3, LOW);
    digitalWrite (OVERLOAD, LOW);
    if (squidSound == 1)
    {
      squidSound = 0;
      delay (30);
    }
    break;
    
    case 3:
    digitalWrite (SQUIDLIGHT1, HIGH);
    digitalWrite (SQUIDLIGHT2, HIGH);
    digitalWrite (SQUIDLIGHT3, HIGH);
    digitalWrite (OVERLOAD, LOW);
    if (squidSound == 0)
    {
      digitalWrite (FULL_SHEILD, HIGH);
      delay (30);
      digitalWrite (FULL_SHEILD, LOW);
      squidSound = 1;
    }
    break;
    case 4:
    digitalWrite (OVERLOAD, HIGH);
    digitalWrite (SQUIDLIGHT1, squidAlert);
    digitalWrite (SQUIDLIGHT2, squidAlert);
    digitalWrite (SQUIDLIGHT3, squidAlert);
    if ((millis() - alertTime)>=alertDelay)
    {
      squidAlert = !squidAlert;
      alertTime = millis();
    }
    break;
    
    default:
    digitalWrite (SQUIDLIGHT1, LOW);
    digitalWrite (SQUIDLIGHT2, LOW);
    digitalWrite (SQUIDLIGHT3, LOW);
    digitalWrite (OVERLOAD, LOW);
    squidSound = 0;
    break;
  }
  
    //set mode behaviours
  switch (engineMode)
  {
    case 0:
    for (int i = ENGINELIGHT1; i<= ENGINECHASER3; i++)
    {
      digitalWrite (i, LOW);
    }
    movingSound = 0;
    fullSpeedSound = 0;
    break;
    
    case 1:
    digitalWrite (ENGINELIGHT1, HIGH);
    digitalWrite (ENGINELIGHT2, LOW);
    digitalWrite (ENGINELIGHT3, LOW);
    digitalWrite (ENGINELIGHT4, LOW);
    digitalWrite (ENGINELIGHT5, LOW);
    digitalWrite (ENGINECHASER1, LOW);
    digitalWrite (ENGINECHASER2, LOW);
    digitalWrite (ENGINECHASER3, LOW);
    movingSound = 0;
    fullSpeedSound = 0;
    break;
    
    case 2:
    digitalWrite (ENGINELIGHT1, HIGH);
    digitalWrite (ENGINELIGHT2, HIGH);
    digitalWrite (ENGINELIGHT3, LOW);
    digitalWrite (ENGINELIGHT4, LOW);
    digitalWrite (ENGINELIGHT5, LOW);
    digitalWrite (ENGINECHASER1, LOW);
    digitalWrite (ENGINECHASER2, LOW);
    digitalWrite (ENGINECHASER3, LOW);
    if (movingSound == 1)
    {
      movingSound = 0;
      delay (30);
    }
    fullSpeedSound = 0;
    break;
    
    case 3:
    digitalWrite (ENGINELIGHT1, HIGH);
    digitalWrite (ENGINELIGHT2, HIGH);
    digitalWrite (ENGINELIGHT3, HIGH);
    digitalWrite (ENGINELIGHT4, LOW);
    digitalWrite (ENGINELIGHT5, LOW);
    fanDelay = 500;
    if (movingSound == 0)
    {
      digitalWrite (MOVING_NOW, HIGH);
      delay (30);
      digitalWrite (MOVING_NOW, LOW);
      movingSound = 1;
    }
    fullSpeedSound = 0;
    break;
    
    case 4:
    digitalWrite (ENGINELIGHT1, HIGH);
    digitalWrite (ENGINELIGHT2, HIGH);
    digitalWrite (ENGINELIGHT3, HIGH);
    digitalWrite (ENGINELIGHT4, HIGH);
    digitalWrite (ENGINELIGHT5, LOW);
    fanDelay = 300;
    if (fullSpeedSound == 1)
    {
      fullSpeedSound = 0;
      delay (30);
    }
    
    break;
    
    case 5:
    digitalWrite (ENGINELIGHT1, HIGH);
    digitalWrite (ENGINELIGHT2, HIGH);
    digitalWrite (ENGINELIGHT3, HIGH);
    digitalWrite (ENGINELIGHT4, HIGH);
    digitalWrite (ENGINELIGHT5, HIGH);
    fanDelay = 100;
    if (fullSpeedSound == 0)
    {
      digitalWrite (FULL_SPEED, HIGH);
      delay (30);
      digitalWrite (FULL_SPEED, LOW);
      fullSpeedSound = 1;
    }
    break;
    default:
    for (int i = ENGINELIGHT1; i<= ENGINECHASER3; i++)
    {
      digitalWrite (i, LOW);
    }
    break;
  }
  
  if (engineMode>=3)
  {
    if ((millis()-fanTime)>fanDelay)
      {
        digitalWrite (fanLED, LOW);
        fanLED++;
          if (fanLED > ENGINECHASER3)
          {
            fanLED = ENGINECHASER1;
          }
        digitalWrite (fanLED, HIGH);
        fanTime = millis();
      }
    
  }
  
  //timer count down
  if (timer > 12000UL)
    digitalWrite (TIMER5, LOW);
  if (timer > 24000UL)
    digitalWrite (TIMER4, LOW);
  if (timer > 36000UL)
    digitalWrite (TIMER3, LOW);
  if (timer > 48000)
  {
    digitalWrite (TIMER2, LOW);
    digitalWrite (47,LOW);
  }
  

  if (timer > 60000UL)
  {
    digitalWrite (TIMER1, LOW);
    digitalWrite (OVERLOAD, LOW);
    for (int i = VENT; i >= SQUIDLIGHT1; i--)
    {
      digitalWrite(i, LOW);
      delay(200);
    }
    start = 0;
    softReset();
  }
}


  void softReset() //software reset
{
  delay(100);
  asm volatile ("  jmp 0");
}

