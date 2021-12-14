#include <avr/interrupt.h>
#include "Arduino.h"

class Tamagotchi {
    private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    float smoothedVolt;
    unsigned int tickCounter;
    volatile float VoltageData_V;        //Battery Voltage Value
    const float VoltageDetection = 7.00;
    boolean VoltageDetectionStatus = false;
    

    // volatile because compiler should not optimize them
    volatile byte flag_save;
    volatile byte flag_is_pet;
    volatile byte flag_is_fed;
    volatile byte flag_read_battery;

    public:
    Tamagotchi();
    void init();
    void onTick();
    void loop();
    void setup();

    private:
    void writeDataToEEPROM();
    void writeToEEPROM(int address, int value);
    void readDataFromEEPROM();
    float readBatteryLevel();
    void debug(String s);
};

extern Tamagotchi myTamagotchi;