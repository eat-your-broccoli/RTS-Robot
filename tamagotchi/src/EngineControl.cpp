#include "EngineControl.h"
#include "DeviceDriverSet_xxx0.h"

// comment this out to use engine , or set it to 0
#define DEBUG_BLOCK_ENGINE 0
#define DEBUG_ENGINE_MODE 1

EngineControl myEngine;
DeviceDriverSet_Motor motor;

void EngineControl::init() {

    #if DEBUG_ENGINE_MODE == 0
        motor.DeviceDriverSet_Motor_Init();
    #else 
        pinMode(PIN_Motor_PWMA, OUTPUT);
        pinMode(PIN_Motor_PWMB, OUTPUT);
        pinMode(PIN_Motor_AIN_1, OUTPUT);
        pinMode(PIN_Motor_BIN_1, OUTPUT);
        pinMode(PIN_Motor_STBY, OUTPUT);
    #endif
}

void EngineControl::stop() {
    Serial.println("stop");
    move(3, 0, 3, 0);
}

void EngineControl::move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB) {
    #if defined DEBUG_BLOCK_ENGINE && DEBUG_BLOCK_ENGINE == 1
        return;
    #endif

    #if DEBUG_ENGINE_MODE == 0
        motor.DeviceDriverSet_Motor_control(dirA, speedA, dirB, speedB, true);
    #else 
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
    #endif
    
    
}

void EngineControl::turn(bool isRight, uint8_t speed) {
    if(isRight == 1) move(2, speed, 1, speed);
    else move(1, speed, 2, speed);
}

void EngineControl::turn90deg(bool isRight) {
    turn(isRight, 130);
    delay(500);
    stop();
}