#include "Tamagotchi.h"
#include <EEPROM.h>
#include "Face_Bitmaps.h"

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
#define VOLT_BATTERY_LOWER_LIMIT 4.0

#define FACE_UPDATE_COOLDOWN 10000

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
    this->affection = 100;
    this->previousMillisFeeding = 0; // will store last time robot was fed
};

void Tamagotchi::init(TwoWire *twi) {
    Serial.println("reading data from EEPROM...");
    // readDataFromEEPROM();
    Serial.println("done reading data from EEPROM");

       
    Serial.println("init AppVoltage ...");
    pinMode(PIN_Voltage, INPUT);
    this->smoothedVolt = readBatteryLevel();
    Serial.println("done init AppVoltage");

    Serial.println("Init display");
        // init display with passed I²C connection
    this->display = Adafruit_SSD1306(128, 64, twi);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed.\nProgram execution will halt (Is the display connected correctly? Are large vars stored in Flash memory (full RAM causes I²C to 'time out')?)"));
        for(;;); // Don't proceed, loop forever
    }
    displayFace(enum_face::awake);
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

    if(this->flag_update_display == 0 && ((currentMillis - this->ts_face_update) > FACE_UPDATE_COOLDOWN || currentMillis < this->ts_face_update)) {
        chooseFace();
    }
    
    if(this->flag_update_display != 0) {
        displayFace(this->display_index);
    }
    // reset flags
    this->flag_is_pet = 0;
    this->flag_is_fed = 0;
    this->flag_read_battery = 0;
    this->flag_update_display = 0;
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

/**
 * @brief displays the face
 * NOTE: DO NOT CALL THIS METHOD DIRECTLY
 * NOTE: use setDisplayFace
 * 
 * direct method calls may be overwritten in the same cycle, rendering them redundant and wasting processing power
 * 
 * @param index - the index of the face. see [enum_faces]
 */
void Tamagotchi::displayFace(enum_face index) {
    // if we're already displaying the face, leave it that way
    if(this->display_index_current == index) return;

    // update var which stores the current displayed index
    this->display_index_current = index;
    // update timestamp when last face was updated
    this->ts_face_update = millis();

    this->display.clearDisplay();
    switch (index) {
        // we're mapping init case to to happy
        case enum_face::init: {
            display.drawBitmap(0,0, FACE_HAPPY, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::happy: {
            display.drawBitmap(0,0, FACE_HAPPY, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::neutral: {
            display.drawBitmap(0,0, FACE_NEUTRAL, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::sad: {
            display.drawBitmap(0,0, FACE_SAD, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::awake: {
            display.drawBitmap(0,0, FACE_AWAKE, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::sleepy: {
            display.drawBitmap(0,0, FACE_SLEEPY, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        case enum_face::hungry: {
            display.drawBitmap(0,0, FACE_HUNGRY, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
            break;
        }
        default: 
            Serial.println("UNKNOWN FACE !!!");
    }
    // flush buffer and display data
    this->display.display();
}

/**
 * @brief sets the display to a certain face
 * NOTE: ONLY SETS if has higher priority
 * 
 * @param index the face that is to be displayed
 * @param priority the priority of the face
 */
void Tamagotchi::setDisplayFace(enum_face index, uint8_t priority) {
    if(priority > this->flag_update_display) {
        this->flag_update_display = priority;
        this->display_index = index;
    }
}

void Tamagotchi::chooseFace() {
    if(this->sleepyness < 5) {
        setDisplayFace(enum_face::sleepy, 20);
        return;
    }
    if(this->hunger >= 70) {
        setDisplayFace(enum_face::hungry, 19);
        return;
    }
    if(this->affection < 20) {
        setDisplayFace(enum_face::sad, 18);
        return;
    }
    if(this->affection < 50) {
        setDisplayFace(enum_face::neutral, 17);
        return;
    }
    setDisplayFace(enum_face::happy, 0);
}

void Tamagotchi::setIsFedFlag()
{
    Serial.println("Button Click detected");
    this->flag_is_fed = 1;
}