#include <avr/interrupt.h>
#include "Arduino.h"
#include "UltrasonicSensor.h"
#include "EngineControl.h"
#include "ServoControl.h"
#include "InstructionSet.h"

#ifndef SPEED_SLOW
#define SPEED_SLOW 75
#define SPEED_NORMAL 120
#define SPEED_FAST 200
#endif

class Tamagotchi
{
private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    float smoothedVolt;
    unsigned int tickCounter;
    volatile float VoltageData_V;        //Battery Voltage Value
    const float VoltageDetection = 7.00;
    boolean VoltageDetectionStatus = false;
    
    unsigned long previousMillisFeeding;

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
    int move_instructionSetIndex = 0;
    // the instruction as a pointer
    InstructionSet *move_instructionSet;
    // which instruction in the instruction set currently is used
    int move_instructionIndex = 0;
    

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
    void setup();
    void setIsFedFlag();

private:
    void writeDataToEEPROM();
    void writeToEEPROM(int address, int value);
    void readDataFromEEPROM();
    float readBatteryLevel();
    uint8_t convertVoltToSleepyness(float voltage);
    void debug(String s);
    void organicMovement();
    void findUnblockedDirection();
    void stopOrganicMovement();
    boolean isDistInRange(unsigned int dist, unsigned int min, unsigned int max);
    InstructionSet* randomInstructionSet();
};

extern Tamagotchi myTamagotchi;