
#include "Arduino.h"

class EngineControl {
    public:
    void init();
    void move(bool dirA, uint8_t speedA, bool dirB, uint8_t speedB);
    void turn(bool isRight, uint8_t speed);
    void stop();

    private:
    #define PIN_Motor_PWMA 5
    #define PIN_Motor_PWMB 6
    #define PIN_Motor_BIN_1 8
    #define PIN_Motor_AIN_1 7
    #define PIN_Motor_STBY 3
};

extern EngineControl myEngine;