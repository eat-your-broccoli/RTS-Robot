#include "EngineControl.h"


EngineControl myEngine;

void EngineControl::init() {
    pinMode(PIN_Motor_PWMA, OUTPUT);
    pinMode(PIN_Motor_PWMB, OUTPUT);
    pinMode(PIN_Motor_AIN_1, OUTPUT);
    pinMode(PIN_Motor_BIN_1, OUTPUT);
    pinMode(PIN_Motor_STBY, OUTPUT);
}

void EngineControl::stop() {
    move(3, 0, 3, 0);
}

void EngineControl::move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB) {
    digitalWrite(PIN_Motor_STBY, HIGH);
    
    // motor group A
    switch (dirA) {
        case 1: 
            digitalWrite(PIN_Motor_AIN_1, HIGH); 
            analogWrite(PIN_Motor_PWMA, speedA);
            break;
        case 2: 
            digitalWrite(PIN_Motor_AIN_1, LOW);
            analogWrite(PIN_Motor_PWMA, speedA);
            break;
        default: 
            analogWrite(PIN_Motor_PWMA, 0);
            digitalWrite(PIN_Motor_STBY, LOW);
    }

    // motor group B
    switch (dirB) {
        case 1: 
            digitalWrite(PIN_Motor_BIN_1, HIGH); 
            analogWrite(PIN_Motor_PWMB, speedB);
            break;
        case 2: 
            digitalWrite(PIN_Motor_BIN_1, LOW);
            analogWrite(PIN_Motor_PWMB, speedB);
            break;
        default: 
            analogWrite(PIN_Motor_PWMB, 0);
            digitalWrite(PIN_Motor_STBY, LOW);
    }
}

void EngineControl::turn(bool isRight, uint8_t speed) {
    if(isRight == 1) move(1, speed, 2, speed);
    else move(2, speed, 1, speed);

}