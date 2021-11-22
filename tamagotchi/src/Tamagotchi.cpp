#include "Tamagotchi.h"

#ifndef DEBUG
// comment next line out if you don't want debug messages
// #define DEBUG
#endif

#ifndef TICK_COUNTER_MAX
#define TICK_COUNTER_MAX 3600
#endif

/**
 * @brief Tamagotchi class
 * 
 * Controls the robot in a pet-like manner
 * includes sentiment like hunger, sleepyness and affection
 * 
 */


Tamagotchi myTamagotchi;

Tamagotchi::Tamagotchi() {
    debug("instance of Tamagotchi created");
    this->sleepyness = 50;
    this->hunger = 50;
    this->affection = 20;
};

void Tamagotchi::init() {
    debug("reading data from EEPROM...");
    readDataFromEEPROM();
    debug("done reading data from EEPROM");
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
    // if(tickCounter % 10 != 0) return;

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
    debug("loop executed");
    int c = 0;

    if(this->flag_read_battery) {
        c++;
        debug("reading battery level");
        // TODO read battery level
        // TODO convert battery level to sleepyness
    }

    if(this->flag_is_fed) {
        c++;
        debug("pet was fed");
        // TODO decrease hunger
        // TODO display feeding symbol
    }

    if(this->flag_is_pet) {
        c++;
        debug("pet was petted");
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

void Tamagotchi::readDataFromEEPROM() {

}

void Tamagotchi::writeDataToEEPROM() {

}

void Tamagotchi::readBatteryLevel() {

}

void Tamagotchi::debug(String s) {
    # ifdef DEBUG
    Serial.println(s);
    #endif
}