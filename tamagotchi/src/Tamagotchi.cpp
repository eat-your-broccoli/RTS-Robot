#include "Tamagotchi.h"
#include <EEPROM.h>

#include "DeviceDriverSet_xxx0.h"

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

#define VOLT_SMOOTHING 0.2
#define VOLT_BATTERY_UPPER_LIMIT 7.0
#define VOLT_BATTERY_LOWER_LIMIT 5.0


DeviceDriverSet_Voltage AppVoltage2;
// feeding cooldown
#define FEEDING_COOLDOWN 10000 // 10 seconds

/**
 * @brief Tamagotchi class
 * 
 * Controls the robot in a pet-like manner
 * includes sentiment like hunger, sleepyness and affection
 * 
 */

Tamagotchi myTamagotchi;

Tamagotchi::Tamagotchi()
{
    this->sleepyness = 50;
    this->hunger = 50;
    this->affection = 20;
    this->previousMillisFeeding = 0; // will store last time robot was fed
};

void Tamagotchi::init()
{
    Serial.println("reading data from EEPROM...");
    readDataFromEEPROM();
    Serial.println("done reading data from EEPROM");

       
    Serial.println("init AppVoltage ...");
    pinMode(PIN_Voltage, INPUT);
    this->smoothedVolt = readBatteryLevel();
    Serial.println("done init AppVoltage");
}

/**
 * @brief gets called every one second
 * 
 */
void Tamagotchi::onTick()
{
    tickCounter++;
    // reset tick counter every TICK_COUNTER_MAX seconds
    if (tickCounter > TICK_COUNTER_MAX)
        tickCounter = 0;

    // if the tick counter is not a multiple of 10 seconds return immediately
    // this way further evaluations only have to be done every 10 seconds
    if (tickCounter % 10 != 0) {
        return;
    }

    // this code will execute every 10 seconds
    Serial.println("Hello every 10 seconds");
    this->hunger++;
    if (this->hunger > 100)
        this->hunger = 100;
    this->affection--;
    if (this->affection < 0)
        this->affection = 0;

    this->flag_read_battery = 1;

    if (tickCounter % 60 == 0)
    {
        Serial.println("Hello every 60 seconds");

        // because it's a multiple of 60 seconds, we place it here so cpu only has to evaluate it every 60 seconds (instead of every 10)
        if (tickCounter % 600 == 0)
        {
            Serial.println("Hello every 600 seconds");
        }
    }
}

void Tamagotchi::loop()
{
    int hasAnythingChanged = 0;
    unsigned long currentMillis = millis();

    if (this->flag_read_battery)
    {
        hasAnythingChanged++;
        float batteryLevel = readBatteryLevel();
        this->smoothedVolt = this->smoothedVolt * (1 - VOLT_SMOOTHING) + (VOLT_SMOOTHING * batteryLevel);
        this->sleepyness = convertVoltToSleepyness(this->smoothedVolt);
        Serial.print("battery = ");
        Serial.println(batteryLevel);
        Serial.print("sleepyness = ");
        Serial.println(sleepyness);
        
        this->flag_read_battery = 0;
    }

    if (this->flag_is_fed)
    {
        
        if ((currentMillis - previousMillisFeeding >= FEEDING_COOLDOWN) || previousMillisFeeding == 0 || currentMillis < previousMillisFeeding)
        {
            // save the last time you fed
            previousMillisFeeding = currentMillis;
            Serial.println("feeding");
            hasAnythingChanged++;
            // Decrease hunger
            if (this->hunger < 20)
            {
                this->hunger = 0;
                Serial.println("Feeding...Pet is full! Hunger at 0...");
            }
            else
            {
                this->hunger = this->hunger - 20;
                Serial.print("Feeding... Hunger at: "); Serial.println(this->hunger);
            }
            // TODO display feeding symbol
        }
        // reset flags
        this->flag_is_fed = 0;
    }

    if (this->flag_is_pet)
    {
        hasAnythingChanged++;
        // TODO increase affection
        // TODO display happy symbol
        // reset flags
        this->flag_is_pet = 0;
    }

    if (hasAnythingChanged > 0)
    {
        Serial.print("Hunger: ");
        Serial.println(this->hunger);
        Serial.print("Affection: ");
        Serial.println(this->affection);
        Serial.print("Sleepyness: ");
        Serial.println(this->sleepyness);
    }
}

/**
 * @brief reads values from EEPROM 
 * NOTE: https://forum.arduino.cc/t/eeprom-default-values/555256
 * EEPROM values are all 0xFF when factory new
 * 
 */
void Tamagotchi::readDataFromEEPROM()
{
    this->sleepyness = EEPROM.read(ADDR_TAMAGOTCHI_SLEEP);
    this->affection = EEPROM.read(ADDR_TAMAGOTCHI_AFFECTION);
    this->hunger = EEPROM.read(ADDR_TAMAGOTCHI_HUNGER);

    Serial.println("read values: ");
    Serial.print("Hunger: ");
    Serial.println(this->hunger);
    Serial.print("Affection: ");
    Serial.println(this->affection);
    Serial.print("Sleepyness: ");
    Serial.println(this->sleepyness);

    if (this->sleepyness == 0xFF)
        this->sleepyness = DEFAULT_SLEEPYNESS;
    if (this->affection == 0xFF)
        this->affection = DEFAULT_AFFECTION;
    if (this->hunger == 0xFF)
        this->hunger = DEFAULT_HUNGER;

    Serial.println("corrected values: ");
    Serial.print("Hunger: ");
    Serial.println(this->hunger);
    Serial.print("Affection: ");
    Serial.println(this->affection);
    Serial.print("Sleepyness: ");
    Serial.println(this->sleepyness);
}

void Tamagotchi::writeDataToEEPROM()
{
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
void Tamagotchi::writeToEEPROM(int address, int value)
{
    EEPROM.update(address, value);
}

/**
 * @brief reads battery level
 */
float Tamagotchi::readBatteryLevel() {
    float Voltage = (analogRead(PIN_Voltage) * 0.0375);
    Voltage = Voltage + (Voltage * 0.08); //Compensation 8%
    return Voltage;
}

uint8_t Tamagotchi::convertVoltToSleepyness(float voltage) {
    if(voltage > VOLT_BATTERY_UPPER_LIMIT) return 100;
    if(voltage < VOLT_BATTERY_LOWER_LIMIT) return 0;
    return (uint8_t) (((voltage - VOLT_BATTERY_LOWER_LIMIT) / (VOLT_BATTERY_UPPER_LIMIT - VOLT_BATTERY_LOWER_LIMIT)) * 100);
}

void Tamagotchi::setIsFedFlag()
{
    Serial.println("Button Click detected");
    this->flag_is_fed = 1;
}