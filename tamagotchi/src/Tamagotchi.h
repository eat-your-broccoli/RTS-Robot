#include <avr/interrupt.h>
#include "Arduino.h"
#include "UltrasonicSensor.h"
#include "EngineControl.h"
#include "ServoControl.h"


class Tamagotchi {
    private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    unsigned int tickCounter;

    // if movement is blocked, e.g. obstacle detected by Ultrasonic sensor
    bool isMovementBlocked = false;

    // timestamp for handlich unblocking routine
    unsigned long ts_blocked;

    unsigned int blocked_instructionIndex;

    // when was last action done (petting, feeding, sleeping)
    unsigned long ts_move_cooldown; 
    // is robot currently moving organically?
    int isOrganicMovement = 0;
    // when started last movement instruction?
    unsigned long ts_move_instruction = 0;
    // which instruction set currently is used
    int move_instructionSet = 0;
    // which instruction in the instruction set currently is used
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
    void findUnblockedDirection();
    void stopOrganicMovement();
    boolean Tamagotchi::isDistInRange(unsigned int dist, unsigned int min, unsigned int max);
};

extern Tamagotchi myTamagotchi;