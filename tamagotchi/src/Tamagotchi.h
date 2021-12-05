#include <avr/interrupt.h>
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef SCREEN_ADDRESS
#define SCREEN_ADDRESS 0x3C
#endif

class Tamagotchi {
    private:
    volatile int affection;
    volatile int hunger;
    volatile int sleepyness;
    unsigned int tickCounter;

    // volatile because compiler should not optimize them
    volatile byte flag_save;
    volatile byte flag_is_pet;
    volatile byte flag_is_fed;
    volatile byte flag_read_battery;

    // SSD1306 display 
    // reference to the display itself
    Adafruit_SSD1306 display;

    // dispaly vars
    // index of the face that is displayed
    unsigned int display_index;

    public:
    Tamagotchi();
    void init(TwoWire *twi);
    void onTick();
    void loop();

    private:
    void writeDataToEEPROM();
    void writeToEEPROM(int address, int value);
    void readDataFromEEPROM();
    void readBatteryLevel();
    void debug(String s);
    void displayFace(unsigned int index);
};

extern Tamagotchi myTamagotchi;