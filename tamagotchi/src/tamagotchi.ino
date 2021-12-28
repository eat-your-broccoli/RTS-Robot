/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-12-18 14:14:35
 * @LastEditors: Changhua
 * @Description: Smart Robot Car V4.0
 * @FilePath: 
 */
#include <avr/wdt.h>
// #include "ApplicationFunctionSet_xxx0.h"
#include <Wire.h>
#include "ApplicationFunctionSet_xxx0.h"
#include "Tamagotchi.h"
#include "DeviceDriverSet_xxx0.h"

// will be incremented every 1 sec
volatile unsigned int timer_counter = 0;
unsigned long lastTimestamp = 0;
unsigned int timer_minute = 60;

unsigned int timer_three_minutes = 3 * 60;

// clear timer every 10 minutes
unsigned int timer_counter_max = 60 * 10;

#define MAX_SERVOS  4

#define PIN_FEEDING_BUTTON 18

#define BAUD_RATE 9600


DeviceDriverSet_IRrecv irRemote;

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.print("Serial init at ");
  Serial.println(BAUD_RATE);
  
  // setupTimers();
  myTamagotchi.init(&Wire);
  wdt_enable(WDTO_2S);

  pinMode(PIN_FEEDING_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_FEEDING_BUTTON), interruptFeedingButton, RISING);

  irRemote.DeviceDriverSet_IRrecv_Init();
}

void interruptFeedingButton(){
  myTamagotchi.setIsFedFlag();
}

void loop()
{
  //put your main code here, to run repeatedly :
  wdt_reset();
  // poorMansTimer();

  // myTamagotchi.loop();

  uint8_t x = 0;
  irRemote.DeviceDriverSet_IRrecv_Get(&x);
  if(x > 0) {
    Serial.println((String) "received: "+x);
  }

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

  // 100 Hz (16000000/((624+1)*256))
  OCR1A = 624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 256
  TCCR1B |= (1 << CS12);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
  Serial.println("Setup timer complete");
}

// ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
// {
//   myTamagotchi.onTick();
// }

void poorMansTimer() {
  unsigned long time = millis();
  if(time - lastTimestamp >= 1000) {
    lastTimestamp = time;
    myTamagotchi.onTick();
  }
}