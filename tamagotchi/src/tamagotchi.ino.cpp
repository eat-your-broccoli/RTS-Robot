# 1 "C:\\Users\\Sven\\AppData\\Local\\Temp\\tmp_ntlbvds"
#include <Arduino.h>
# 1 "C:/Users/Sven/Documents/GitHub/RTS-Robot/tamagotchi/src/tamagotchi.ino"
# 9 "C:/Users/Sven/Documents/GitHub/RTS-Robot/tamagotchi/src/tamagotchi.ino"
#include <avr/wdt.h>
#include <Wire.h>
#include "Tamagotchi.h"

#define PIN_FEEDING_BUTTON 18
#define PIN_SLEEPING_BUTTON 19

#define BAUD_RATE 9600
void interruptSleepButton();
void setup();
void sleepingAttachInterrupt();
void interruptFeedingButton();
void loop();
void setupTimers();
#line 17 "C:/Users/Sven/Documents/GitHub/RTS-Robot/tamagotchi/src/tamagotchi.ino"
void interruptSleepButton(){
  sleepingAttachInterrupt();
  Serial.println("wakup");
}

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.print("Serial init at ");
  Serial.println(BAUD_RATE);

  setupTimers();
  myTamagotchi.init(&Wire);
  wdt_enable(WDTO_2S);

  pinMode(PIN_FEEDING_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FEEDING_BUTTON), interruptFeedingButton, RISING);
   attachInterrupt(digitalPinToInterrupt(PIN_SLEEPING_BUTTON),interruptSleepButton , RISING);
}





void sleepingAttachInterrupt(){
   attachInterrupt(digitalPinToInterrupt(PIN_FEEDING_BUTTON), interruptFeedingButton, RISING);
}
void interruptFeedingButton(){
  myTamagotchi.setIsFedFlag();
}

void loop()
{

  wdt_reset();
  myTamagotchi.loop();
}




void setupTimers() {

  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;


  OCR1A = 15624;

  TCCR1B |= (1 << WGM12);

  TCCR1B |= (1 << CS12) | (1 << CS10);

  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  myTamagotchi.onTick();
}