
#include "Arduino.h"

class EngineControl {
    public:
    void init();
    void move(uint8_t dirA, uint8_t speedA, uint8_t dirB, uint8_t speedB);
    void turn(bool isRight, uint8_t speed);
    void stop();
    void turn90deg(bool isRight);
    void forward(uint8_t speed);
    void backward(uint8_t speed);
    

    private:
    #define PIN_Motor_PWMA 5
    #define PIN_Motor_PWMB 6
    #define PIN_Motor_BIN_1 8
    #define PIN_Motor_AIN_1 7
    #define PIN_Motor_STBY 3
};

extern EngineControl myEngine;