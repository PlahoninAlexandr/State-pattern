/*
Plakhonin Alexandr
Variant 9, lab 1, Ex 3.2.2 state pattern with millis()
*/
#include <Arduino.h>
#include <String>

// =================================================================
// классы

static unsigned long curT = millis();

unsigned long a = random(0, 2000);
unsigned long b = random(0, 2000);
unsigned long c = random(0, 2000);

unsigned long currentTime = millis();
unsigned long interval = 2 * (a + 200);
unsigned long interval2 = b + 150;
unsigned long interval3 = 4 * (c + 100);

class Flasher
{
  int ledPin;
  long OnTime;
  long OffTime;

  int ledState;
  unsigned long previousMillis;
  
  public:
  Flasher(int pin, long on, long off)         // Constructor instantiates Flasher and initializes class member variables and state
  {
   ledPin = pin;
   pinMode(ledPin, OUTPUT);

   OnTime = on;
   OffTime = off;
 
   ledState = LOW;
   previousMillis = 0;
  }

  void Update()                               // We find out if the time has come to change the state of the LED 
  {
   unsigned long currentMillis = millis();
 
   if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
   {
     ledState = LOW;                          // turn off            
     previousMillis = currentMillis;          // remember the moment in time
     digitalWrite(ledPin, ledState);          // implement the new state
   }
   else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
   {
     ledState = HIGH;
     previousMillis = currentMillis ;
     digitalWrite(ledPin, ledState);
   }
  }
};

Flasher led1(D1, a, 200);
Flasher led2(D2, b, 150);
Flasher led3(D3, c, 100);


class Context;

class State{
  public:
  State(){}
  virtual ~State(){}
  virtual void firstLed(Context *context);
  virtual void secondLed(Context *context);
  virtual void thirdLed(Context *context);
};

class Context{
  private:
  State *current;

  public:
  Context(State *state) : current{state}{}
  ~Context() {}

  void set_current_state(State* state) {
    current = state;
  }

  void firstLed() {
    current->firstLed(this);
  }

  void secondLed() {
    current->secondLed(this);
  }

  void thirdLed() {
    current->thirdLed(this);
  }
};

// =================================================================
// заглушки

void State::firstLed(Context *context){
  led1.Update();
  return;
}

void State::secondLed(Context *context){
  led2.Update();
  return;
}

void State::thirdLed(Context *context){
  led3.Update();
  return;
}

// ================================================================
// классы состояний

class First_state : public State{
  public:
  First_state(){}
  ~First_state(){}

  void firstLed(Context *context);
  void secondLed(Context *context);
  void thirdLed(Context *context);
};

class Second_state : public State{
  public:
  Second_state(){}
  ~Second_state(){}

  void firstLed(Context *context);
  void secondLed(Context *context);
  void thirdLed(Context *context);
};

class Third_state : public State{
  public:
  Third_state(){}
  ~Third_state(){}

  void firstLed(Context *context);
  void secondLed(Context *context);
  void thirdLed(Context *context);
};

// ===============================================================
// варианты переходов из состояния в состояние


// переходы из состояния First_state

void First_state::firstLed(Context *context){
  State::firstLed(context);
  return;
}

void First_state::secondLed(Context *context){
  led2.Update();
  Second_state* new_state = new Second_state();
  context->set_current_state(new_state);
  delete this;
}

void First_state::thirdLed(Context *context){
  State::firstLed(context);
  return;
}

// переходы из состояния Second_state

void Second_state::secondLed(Context *context){
  State::secondLed(context);
  return;
}

void Second_state::thirdLed(Context *context){
  led3.Update();
  Third_state* new_state = new Third_state();
  context->set_current_state(new_state);
  delete this;
}

void Second_state::firstLed(Context *context){
  State::secondLed(context);
  return;
}

// переходы из состояния Third_state

void Third_state::thirdLed(Context *context){
  State::thirdLed(context);
  return;
}

void Third_state::firstLed(Context *context){
  led1.Update();
  First_state* new_state = new First_state();
  context->set_current_state(new_state);
  delete this;
}

void Third_state::secondLed(Context *context){
  State::thirdLed(context);
  return;
}

Context cont(new Third_state());

void setup(){
}

void loop(){
  if (millis() - currentTime < interval){
    cont.firstLed();
  }
  if((millis() - currentTime > interval) && (millis() - currentTime < (interval + interval2))){
    cont.secondLed();
  }
  if((millis() - currentTime > (interval + interval2)) && (millis() - currentTime < (interval + interval2 + interval3))){
    cont.thirdLed();
  }
  if(millis() - currentTime > (interval + interval2 + interval3)){
    currentTime = millis();
  }
}