#include "Tamagotchi.h"
#include <EEPROM.h>

#ifndef DEBUG
// comment next line out if you don't want debug messages
// #define DEBUG
#endif

#ifndef TICK_COUNTER_MAX
#define TICK_COUNTER_MAX 3600
#endif

// arbitrary addresses 
#define ADDR_TAMAGOTCHI_HUNGER 341
#define ADDR_TAMAGOTCHI_SLEEP 342
#define ADDR_TAMAGOTCHI_AFFECTION 343

// default values
#define DEFAULT_SLEEPYNESS 0
#define DEFAULT_HUNGER 80
#define DEFAULT_AFFECTION 20


/**
 * @brief Tamagotchi class
 * 
 * Controls the robot in a pet-like manner
 * includes sentiment like hunger, sleepyness and affection
 * 
 */


Tamagotchi myTamagotchi;

Tamagotchi::Tamagotchi() {
    this->sleepyness = 50;
    this->hunger = 50;
    this->affection = 20;
};

void Tamagotchi::init() {
    Serial.println("reading data from EEPROM...");
    readDataFromEEPROM();
    Serial.println("done reading data from EEPROM");
}

/**
 * @brief gets called every one second
 * 
 */
void Tamagotchi::onTick() {
    tickCounter++;
    // reset tick counter every TICK_COUNTER_MAX seconds
    if(tickCounter > TICK_COUNTER_MAX) tickCounter = 0;

    // if the tick counter is not a multiple of 10 seconds return immediately
    // this way further evaluations only have to be done every 10 seconds
    if(tickCounter % 10 != 0) return;

    // this code will execute every 10 seconds
    // Serial.println("Hello every 10 seconds")
    this->hunger++; 
    if(this->hunger > 100) this->hunger = 100;
    this->affection--; 
    if(this->affection < 0) this->affection = 0;

    this->flag_read_battery = 1;

    if(tickCounter % 60 == 0) {
      // Serial.println("Hello every 60 seconds")
        
        // because it's a multiple of 60 seconds, we place it here so cpu only has to evaluate it every 60 seconds (instead of every 10)
      if(tickCounter % 600 == 0) {
        // Serial.println("Hello every 600 seconds") 
      }  
    }
}

/**
 * @brief executed on every loop of the main program
 * 
 */
void Tamagotchi::loop() {
    int c = 0;

    if(this->flag_read_battery) {
        c++;
        // TODO read battery level
        // TODO convert battery level to sleepyness
    }

    if(this->flag_is_fed) {
        c++;
        // TODO decrease hunger
        // TODO display feeding symbol
    }

    if(this->flag_is_pet) {
        c++;
        // TODO increase affection
        // TODO display happy symbol
    }

    if(c > 0) {
        Serial.print("Hunger: "); Serial.println(this->hunger);
        Serial.print("Affection: "); Serial.println(this->affection);
        Serial.print("Sleepyness: "); Serial.println(this->sleepyness);

    }
    
    // reset flags
    this->flag_is_pet = 0;
    this->flag_is_fed = 0;
    this->flag_read_battery = 0;
}

/**
 * @brief reads values from EEPROM 
 * NOTE: https://forum.arduino.cc/t/eeprom-default-values/555256
 * EEPROM values are all 0xFF when factory new
 * 
 */
void Tamagotchi::readDataFromEEPROM() {
    this->sleepyness = EEPROM.read(ADDR_TAMAGOTCHI_SLEEP);
    this->affection = EEPROM.read(ADDR_TAMAGOTCHI_AFFECTION);
    this->hunger = EEPROM.read(ADDR_TAMAGOTCHI_HUNGER);

    Serial.println("read values: ");
    Serial.print("Hunger: "); Serial.println(this->hunger);
    Serial.print("Affection: "); Serial.println(this->affection);
    Serial.print("Sleepyness: "); Serial.println(this->sleepyness);

    if(this->sleepyness == 0xFF) this->sleepyness = DEFAULT_SLEEPYNESS;
    if(this->affection == 0xFF) this->affection = DEFAULT_AFFECTION;
    if(this->hunger == 0xFF) this->hunger = DEFAULT_HUNGER;

    Serial.println("corrected values: ");
    Serial.print("Hunger: "); Serial.println(this->hunger);
    Serial.print("Affection: "); Serial.println(this->affection);
    Serial.print("Sleepyness: "); Serial.println(this->sleepyness);
}

void Tamagotchi::writeDataToEEPROM() {
    Serial.println("writing values: ");
    writeToEEPROM(ADDR_TAMAGOTCHI_HUNGER, this->hunger);
    writeToEEPROM(ADDR_TAMAGOTCHI_AFFECTION, this->affection);
    writeToEEPROM(ADDR_TAMAGOTCHI_SLEEP, this->sleepyness);
    Serial.println("wrote values to EEPROM");
}

/**
 * @brief write to address in EEPROM
 * only write when written value and newValue deviate above tolerance
 * 
 * @param address 
 * @param value 
 * @param tolerance 
 */
void Tamagotchi::writeToEEPROM(int address, int value) {
    EEPROM.update(address, value);
}

void Tamagotchi::readBatteryLevel() {

}