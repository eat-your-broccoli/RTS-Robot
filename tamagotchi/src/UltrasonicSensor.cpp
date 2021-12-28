#include "UltrasonicSensor.h"
#include "DeviceDriverSet_xxx0.h"

DeviceDriverSet_ULTRASONIC ultra;

#ifndef TRIG_PIN
#define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#endif

#ifndef ECHO_PIN
#define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
#endif

#ifndef MAX_DISTANCE
#define MAX_DISTANCE 200      // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#endif

UltrasonicSensor myUltrasonicSensor;

void UltrasonicSensor::init(void) {
    ultra.DeviceDriverSet_ULTRASONIC_Init();
    // pinMode(ECHO_PIN, INPUT); //Ultrasonic module initialization
    // pinMode(TRIG_PIN, OUTPUT);
}

uint16_t UltrasonicSensor::read() {
    uint16_t val;
    ultra.DeviceDriverSet_ULTRASONIC_Get(&val);
    return val;
    // unsigned int tempda_x = 0;
    // digitalWrite(TRIG_PIN, LOW);
    // delayMicroseconds(2);
    // digitalWrite(TRIG_PIN, HIGH);
    // delayMicroseconds(10);
    // digitalWrite(TRIG_PIN, LOW);
    // noInterrupts();
    // tempda_x = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58);
    // interrupts();
    // // *ULTRASONIC_Get = tempda_x;

    // if (tempda_x > 150)
    // {
    //     return 150;
    // }
    // else
    // {
    //     return tempda_x;
    // }
}
