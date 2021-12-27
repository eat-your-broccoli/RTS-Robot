#include "ServoControl.h"
#include "../lib/Servo/src/Servo.h"
#include "DeviceDriverSet_xxx0.h"

// mode 0: use elegoo code, 1: use own code
#define DEBUG_SERVO_MODE 1
#define DEBUG_SERVO_PRINT 1

ServoControl myServo;
Servo servo;

DeviceDriverSet_Servo s;

void ServoControl::init() {
    #ifdef DEBUG_SERVO_PRINT && DEBUG_SERVO_PRINT == 1
        Serial.println("init servo");
        Serial.println((String) "servo mode: "+DEBUG_SERVO_MODE);
    #endif
    #if DEBUG_SERVO_MODE == 0
        s.DeviceDriverSet_Servo_Init(SERVO_CENTER);
    #else
        servo.attach(SERVO_PIN, 500, 2400); //500: 0 degree  2400: 180 degree
        // servo.attach(SERVO_PIN);
        turn(SERVO_CENTER);
    #endif
}

void ServoControl::turn(unsigned int angle) {
    if(currentAngle == angle) return;
    this->currentAngle = angle;

    #if defined DEBUG_SERVO_PRINT && DEBUG_SERVO_PRINT == 1
        Serial.println((String) "turn servo: "+angle);
    #endif  

    #if DEBUG_SERVO_MODE == 0
        s.DeviceDriverSet_Servo_control(angle);
    #else
        servo.write(angle);
    #endif
}

void ServoControl::reset() {
    // servo.attach(SERVO_PIN);
    turn(SERVO_CENTER);
    // delay(450);
    // servo.detach();
}