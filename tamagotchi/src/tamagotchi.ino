/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2020-12-18 14:14:35
 * @LastEditors: Changhua
 * @Description: Smart Robot Car V4.0
 * @FilePath: 
 */
#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"

// will be incremented every 1 sec
volatile unsigned int timer_counter = 0;

unsigned int timer_minute = 60;

unsigned int timer_three_minutes = 3 * 60;

// clear timer every 10 minutes
unsigned int timer_counter_max = 60 * 10;

#define MAX_SERVOS = 4

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();
  setupTimers();
  wdt_enable(WDTO_2S);
}

void loop()
{
  //put your main code here, to run repeatedly :
  wdt_reset();
  Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();
  Application_FunctionSet.ApplicationFunctionSet_KeyCommand();
  Application_FunctionSet.ApplicationFunctionSet_RGB();
  // Application_FunctionSet.ApplicationFunctionSet_Follow();
  // Application_FunctionSet.ApplicationFunctionSet_Obstacle();
  // Application_FunctionSet.ApplicationFunctionSet_Tracking();
  // Application_FunctionSet.ApplicationFunctionSet_Rocker();
  Application_FunctionSet.ApplicationFunctionSet_Standby();
  Application_FunctionSet.ApplicationFunctionSet_IRrecv();
  Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();

  Application_FunctionSet.CMD_ServoControl_xxx0();
  Application_FunctionSet.CMD_MotorControl_xxx0();
  Application_FunctionSet.CMD_CarControlTimeLimit_xxx0();
  Application_FunctionSet.CMD_CarControlNoTimeLimit_xxx0();
  Application_FunctionSet.CMD_MotorControlSpeed_xxx0();
  Application_FunctionSet.CMD_LightingControlTimeLimit_xxx0();
  Application_FunctionSet.CMD_LightingControlNoTimeLimit_xxx0();
  Application_FunctionSet.CMD_ClearAllFunctions_xxx0();
}

/**
 * @brief setup a timer that fires every one second
 */
void setupTimers() {
  // a great thank you to https://www.arduinoslovakia.eu/application/timer-calculator
  cli(); // disable all interrupts
  TCCR4A = 0;     // set entire TCCR1A register to 0
  TCCR4B = 0;     // same for TCCR1B
  // set compare match register to desired timer count:
  OCR4A = 15624;

  // turn on CTC mode:
  TCCR4B |= (1 << WGM42);

  // 1024 prescaler
  TCCR4B |= (1 << CS42) | (1 << CS40);

  // enable timer compare interrupt:
  TIMSK4 |= (1 << OCIE4A);
  sei();
  Serial.println("Setup timer complete");
}

ISR(TIMER4_COMPA_vect) // timer compare interrupt service routine
{
  timer_counter++;
  if(timer_counter >= timer_counter_max) timer_counter = 0;

  // code that executes every 10 seconds here
  if(timer_counter % (10) == 0) {
    Serial.println("10 seconds");
  }

  // code that executes every minute here
  if(timer_counter % timer_minute == 0) {
    Serial.println("1 minute");
  }

  // code that executes every three minutes here
  if(timer_counter % timer_three_minutes == 0) {
    Serial.println("3 minutes");  
  }
}