#include "ServoControl.h"
#include "../lib/Servo/src/Servo.h"
#include "DeviceDriverSet_xxx0.h"
ServoControl myServo;
Servo servo;

DeviceDriverSet_Servo s;

void ServoControl::init() {
    s.DeviceDriverSet_Servo_Init(SERVO_CENTER);
    // servo.attach(SERVO_PIN, 500, 2400); //500: 0 degree  2400: 180 degree
    // // servo.attach(SERVO_PIN);
    // turn(90);
    // delay(2000);
}

void ServoControl::turn(unsigned int angle) {
    s.DeviceDriverSet_Servo_control(angle);
    // servo.attach(SERVO_PIN);
    // servo.write(angle);
    // delay(450);
    // servo.detach();
}

void ServoControl::reset() {
    // servo.attach(SERVO_PIN);
    servo.write(SERVO_CENTER);
    // delay(450);
    // servo.detach();
}