#include "Arduino.h"
// #include "Servo.h"

#define SERVO_RIGHT 0
#define SERVO_CENTER 90
#define SERVO_LEFT 180
#define SERVO_CENTER_RIGHT 45
#define SERVO_CENTER_LEFT 135



class ServoControl {
    public:
    void init();
    void turn(int angle);
    void reset();
    private: 
    #define SERVO_PIN 10
};

extern ServoControl myServo;