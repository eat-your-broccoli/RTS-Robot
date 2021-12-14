#include <avr/interrupt.h>
#include "Arduino.h"
#include "UltrasonicSensor.h"

class Tamagotchi {
    private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    unsigned int tickCounter;

    // when was last action done (petting, feeding, sleeping)
    unsigned long ts_move_cooldown; 
    // is robot currently moving organically?
    int isOrganicMovement = 0;
    // when started last movement instruction?
    unsigned long ts_move_instruction = 0;
    // which instruction set currently is used
    int move_instructionIndex = 0;

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

    private:
    void writeDataToEEPROM();
    void writeToEEPROM(int address, int value);
    void readDataFromEEPROM();
    void readBatteryLevel();
    void debug(String s);
    void organicMovement();
    void stopOrganicMovement();
};

extern Tamagotchi myTamagotchi;