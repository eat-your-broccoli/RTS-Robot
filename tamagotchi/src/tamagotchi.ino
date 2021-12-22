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
#include "Tamagotchi.h"

// will be incremented every 1 sec
volatile unsigned int timer_counter = 0;

unsigned int timer_minute = 60;

unsigned int timer_three_minutes = 3 * 60;

// clear timer every 10 minutes
unsigned int timer_counter_max = 60 * 10;  

#define MAX_SERVOS = 4
#define BAUD_RATE 9600

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.print("Serial init at ");
  Serial.println(BAUD_RATE);
  
  // put your setup code here, to run once:
  Application_FunctionSet.ApplicationFunctionSet_Init();

  // initialize tamagotchi
  myTamagotchi.init();
  setupTimers();
  wdt_enable(WDTO_2S);
}

void loop()
{
  //put your main code here, to run repeatedly :
  wdt_reset();
  myTamagotchi.loop();
  // Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();
  // Application_FunctionSet.ApplicationFunctionSet_KeyCommand();
  // Application_FunctionSet.ApplicationFunctionSet_RGB();
  // Application_FunctionSet.ApplicationFunctionSet_Follow();
  // Application_FunctionSet.ApplicationFunctionSet_Obstacle();
  // Application_FunctionSet.ApplicationFunctionSet_Tracking();
  // Application_FunctionSet.ApplicationFunctionSet_Rocker();
  // Application_FunctionSet.ApplicationFunctionSet_Standby();
  // Application_FunctionSet.ApplicationFunctionSet_IRrecv();
  // Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();

  // Application_FunctionSet.CMD_ServoControl_xxx0();
  // Application_FunctionSet.CMD_MotorControl_xxx0();
  // Application_FunctionSet.CMD_CarControlTimeLimit_xxx0();
  // Application_FunctionSet.CMD_CarControlNoTimeLimit_xxx0();
  // Application_FunctionSet.CMD_MotorControlSpeed_xxx0();
  // Application_FunctionSet.CMD_LightingControlTimeLimit_xxx0();
  // Application_FunctionSet.CMD_LightingControlNoTimeLimit_xxx0();
  // Application_FunctionSet.CMD_ClearAllFunctions_xxx0();
}

/**
 * @brief setup a timer that fires every one second
 */
void setupTimers() {
  // a great thank you to https://www.arduinoslovakia.eu/application/timer-calculator
  noInterrupts();
  // Clear registers
  TCCR5A = 0;
  TCCR5B = 0;
  TCNT5 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR5A = 15624;
  // CTC
  TCCR5B |= (1 << WGM52);
  // Prescaler 1024
  TCCR5B |= (1 << CS52) | (1 << CS50);
  // Output Compare Match A Interrupt Enable
  TIMSK5 |= (1 << OCIE5A);
  interrupts();
  Serial.println("Setup timer complete");
}

ISR(TIMER5_COMPA_vect) // timer compare interrupt service routine
{
  myTamagotchi.onTick();
}