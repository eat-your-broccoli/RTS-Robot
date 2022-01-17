/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-12-18 14:14:35
 * @LastEditors: Changhua
 * @Description: Smart Robot Car V4.0
 * @FilePath: 
 */
#include <avr/wdt.h>
#include <Wire.h>
#include "Tamagotchi.h"

#define PIN_FEEDING_BUTTON 18
#define PIN_SLEEPING_BUTTON 19 // select free interrupt pin

#define BAUD_RATE 9600
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
   attachInterrupt(digitalPinToInterrupt(PIN_SLEEPING_BUTTON), interruptSleepButton, RISING);
}

void sleepingAttachInterrupt(){
   attachInterrupt(digitalPinToInterrupt(PIN_FEEDING_BUTTON), interruptFeedingButton, RISING);
}
void interruptFeedingButton(){
  myTamagotchi.setIsFedFlag();
}

void loop()
{
  //put your main code here, to run repeatedly :
  wdt_reset();
  myTamagotchi.loop();
}

/**
 * @brief setup a timer that fires every one second
 */
void setupTimers() {
  // a great thank you to https://www.arduinoslovakia.eu/application/timer-calculator
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR1A = 15624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  myTamagotchi.onTick();
}