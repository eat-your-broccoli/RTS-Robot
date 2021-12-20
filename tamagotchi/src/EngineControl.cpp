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
    move(true, 0, true, 0);
}

void EngineControl::move(bool dirA, uint8_t speedA, bool dirB, uint8_t speedB) {
    digitalWrite(PIN_Motor_STBY, HIGH);
    
    // motor group A
    switch (dirA) {
        case true: 
            digitalWrite(PIN_Motor_AIN_1, HIGH); 
            analogWrite(PIN_Motor_PWMA, speedA);
            break;
        case false: 
            digitalWrite(PIN_Motor_AIN_1, LOW);
            analogWrite(PIN_Motor_PWMA, speedA);
            break;
        default: 
            analogWrite(PIN_Motor_PWMA, 0);
            digitalWrite(PIN_Motor_STBY, LOW);
    }

    // motor group B
    switch (dirB) {
        case true: 
            digitalWrite(PIN_Motor_BIN_1, HIGH); 
            analogWrite(PIN_Motor_PWMB, speedB);
            break;
        case false: 
            digitalWrite(PIN_Motor_BIN_1, LOW);
            analogWrite(PIN_Motor_PWMB, speedB);
            break;
        default: 
            analogWrite(PIN_Motor_PWMB, 0);
            digitalWrite(PIN_Motor_STBY, LOW);
    }
}

void EngineControl::turn(bool isRight, uint8_t speed) {
    move(isRight, speed, !isRight, speed);
}