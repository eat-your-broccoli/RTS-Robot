#include <avr/interrupt.h>
#include "Arduino.h"

class Tamagotchi
{
private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    unsigned int tickCounter;
    unsigned long previousMillisFeeding;

    // volatile because compiler should not optimize them
    volatile byte flag_save;
    volatile byte flag_is_pet;
    volatile byte flag_is_fed;
    volatile byte flag_read_battery;
    volatile byte flag_button;

public:
    Tamagotchi();
    void init();
    void onTick();
    void loop();
    void setIsFedFlag();

private:
    void writeDataToEEPROM();
    void writeToEEPROM(int address, int value);
    void readDataFromEEPROM();
    void readBatteryLevel();
    void debug(String s);
};

extern Tamagotchi myTamagotchi;