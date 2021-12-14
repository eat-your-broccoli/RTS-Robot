#include "Arduino.h"

class UltrasonicSensor {
    private: 
    public:
    void init(void);
    uint16_t read(void);
};

extern UltrasonicSensor myUltrasonicSensor;