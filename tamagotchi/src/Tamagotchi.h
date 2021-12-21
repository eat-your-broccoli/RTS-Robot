#include <avr/interrupt.h>
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef SCREEN_ADDRESS
#define SCREEN_ADDRESS 0x3C
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// class because we don't want to pollute namespace. see #3 in https://www.modernescpp.com/index.php/c-core-guidelines-rules-for-enumerations
enum class enum_face {
    init, // display won't update when new index == oldIndex. init is used to prevent this from happening
    happy,
    neutral,
    sad,
    awake,
    sleepy,
    hungry
};

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

    // volatile because compiler should not optimize them
    volatile byte flag_save;
    volatile byte flag_is_pet;
    volatile byte flag_is_fed;
    volatile byte flag_read_battery;
    volatile byte flag_update_display;

    // SSD1306 display 
    // reference to the display itself
    Adafruit_SSD1306 display;

    // dispaly vars
    // index of the face that is displayed
    enum_face display_index = enum_face::init;
    volatile byte flag_button;

public:
    Tamagotchi();
    void init(TwoWire *twi);
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
    void displayFace(enum_face index);
    void setDisplayFace(enum_face index, byte priority);
};

extern Tamagotchi myTamagotchi;