#include "Tamagotchi.h"
#include <EEPROM.h>
#include "Face_Bitmaps.h"
#include "IR_Buttons.h"

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

// organic movement 
#define MOVE_ACTION_COOLDOWN 3000 // 3000 ms = 3sec
#define MOVE_UNBLOCK_COOLDOWN 2500 // 1000ms = 1sec

// voltage reading
#define VOLT_SMOOTHING 0.2
#define VOLT_BATTERY_UPPER_LIMIT 7.0
#define VOLT_BATTERY_LOWER_LIMIT 4.0

#define FACE_UPDATE_COOLDOWN 10000

// feeding cooldown
#define FEEDING_COOLDOWN 10000 // 10 seconds
#define PETTING_COOLDOWN 10000 // 10 seconds


#define PETTING_AFFECTION_INC 15


#define MIN_DISTANCE 20

#define PIN_RANDOM_INIT A10

// set to 1 to debug
#define DEBUG_IR_RECEIVE 0
#define DEBUG_NO_ORGANIC_MOVEMENT 0

// IR Remote
DeviceDriverSet_IRrecv irRemote;

Tamagotchi myTamagotchi;

/**
 * @brief Tamagotchi class
 * 
 * Controls the robot in a pet-like manner
 * includes sentiment like hunger, sleepyness and affection
 * 
 */
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

    pinMode(PIN_RANDOM_INIT, INPUT);
    randomSeed(analogRead(PIN_RANDOM_INIT));

    myUltrasonicSensor.init();
    myEngine.init();
    myServo.init();

    myServo.turn(SERVO_CENTER_RIGHT);
    delay(500);
    myServo.turn(SERVO_CENTER_LEFT);
    delay(500);
    myServo.reset();
       
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

    irRemote.DeviceDriverSet_IRrecv_Init();

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

    // read data from remote
    irReceive();
    irReceiveRoutine();

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
        if(this->affection < 100 && (currentMillis < previousMillisPetting 
        ||  currentMillis - previousMillisPetting >= PETTING_COOLDOWN)) {
            hasAnythingChanged++;
            previousMillisPetting = currentMillis;
            this->affection += 20;
            if(this->affection > 100) this->affection = 100;

            // TODO display happy symbol
            // TODO do animation
        }
        
        this->flag_is_pet = 0;
    }

    if(hasAnythingChanged > 0) {
        Serial.print("Hunger: "); Serial.println(this->hunger);
        Serial.print("Affection: "); Serial.println(this->affection);
        Serial.print("Sleepyness: "); Serial.println(this->sleepyness);
    }

    organicMovement();
    
    
    // reset flags
    this->flag_is_pet = 0;
    this->flag_is_fed = 0;
    this->flag_read_battery = 0;
    
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
 * @brief handles organic movement
 * 
 * 
 * what is a instruction set? a instruction consist of an action 
 * like moving, turning, ... and a timespan during which the instruction has to be done
 */
void Tamagotchi::organicMovement() {
    #if defined DEBUG_NO_ORGANIC_MOVEMENT && DEBUG_NO_ORGANIC_MOVEMENT > 0
        return;
    #endif

    unsigned long time = millis();
    // check if last other action was at least n seconds in the past
    if(this->isOrganicMovement == false &&
     (time < this->ts_move_cooldown || (time - this->ts_move_cooldown) < MOVE_ACTION_COOLDOWN)) {
        // wait until enough time has passed
        return;
    }

    // instruction set can disable distance measure
    if(this->isOrganicMovement == false || this->move_instructionSet->blockDistMeasure != true) {
        // check if obstacle is present
        // if yes, stop. Turn a bit
        uint16_t dist = myUltrasonicSensor.read();
        // if ultrasonic sensor detects obstacle within 20cm of range
        // Serial.println((String) "my distance is "+dist);
        if(this->isMovementBlocked || isDistInRange(dist, 0, MIN_DISTANCE)) {
            // Serial.println("movement is blocked");
            findUnblockedDirection();
            myServo.reset();
            return;
        }
    }
    
    // check if not already moving organically
    if(this->isOrganicMovement == 0) {
        // reset instruction set if it was not properly resetted
        this->move_instructionSetIndex = -1;
        this->ts_move_instruction = 0;
        this->isOrganicMovement = 1;
    }

    // load instruction set
    if(this->move_instructionSetIndex == -1) {
        Serial.println("loading instruction set");
        this->setInstructionSet(this->randomInstructionSet());
    }

    Instruction *instr = (this->move_instructionSet->instr[this->move_instructionIndex]);

    // TODO timer overflows
    // check if current instruction's time has passed
    if(this->ts_move_instruction != 0 && time - this->ts_move_instruction <= (instr->time)) {
        // do nothing
        return;
    }
    // next instruction in set
    this->move_instructionIndex++;
    // if we have done all instructions in the set, stop the movement
    if(this->move_instructionIndex >= this->move_instructionSet->length) {
        stopOrganicMovement();
        return;
    }

    Serial.println("loading next instruction");
    instr = (this->move_instructionSet->instr[this->move_instructionIndex]);

    // decode direction instruction
    uint8_t dirR = (instr->dir & 0b01); // 11 AND 01 = 01 (forward right); 10 AND 01 = 00 (backward right)
    uint8_t dirL = (instr->dir >> 1);  // 11 >> 1 = 01   
    
    // execute instruction
    this->ts_move_instruction = time;
    myEngine.move(dirR, instr->R, dirL, instr->L);

    // if instr is 255, it will turn randomly
    if(instr->servo == 255) myServo.turn(random(15, 165));
    else myServo.turn(instr->servo);
}

void Tamagotchi::stopOrganicMovement() {
    Serial.println("stopping organic movement");
    myEngine.stop();
    this->isOrganicMovement = 0;
    this->move_instructionIndex = 0;
    this->move_instructionSetIndex = -1;
    myServo.reset();
}

/**
 * @brief turns around and finds a direction that is not blocked by an obstacle
 * this function uses delay
 * measures distance to obstacle in three directions (left, center, right)
 * if no direction is free, move backward and turn randomly
 */
void Tamagotchi::findUnblockedDirection() {
  
    // if previously the movement wasn't blocked, stop the engine to avoid hitting obstacle
    if(this->isMovementBlocked == false) {
        myEngine.stop();
        this->ts_move_instruction = 0;
        this->isMovementBlocked = true;
        this->ts_blocked = millis(); 
        // this->blocked_instructionIndex = 0;
        this->fUD_Index =0;
    } 
    else if (millis()- this->ts_blocked  < 200) {
        return;
    } 
    else if (this->fUD_Index== 3) {
        if (millis()>=this->ts_blocked+this->fUD_randomTurnTime){
            this->fUD_Index++;
        } else {
            return;
        }
    } 
    else {
        this->fUD_Index++;
    }

    switch(this->fUD_Index) {
        case 0:
            fUD_Directions[0] = myUltrasonicSensor.read();
            myServo.turn(SERVO_CENTER_RIGHT);
            this->fUD_maxDistIndex = 0;
            this->ts_blocked  = millis();
            return;
        case 1: {
            this->fUD_Directions[1] = myUltrasonicSensor.read();
            if(this->fUD_Directions[1] >= this->fUD_Directions[0]) this->fUD_maxDistIndex = 1;
            myServo.turn(SERVO_CENTER_LEFT);
            this->ts_blocked  = millis();
            return;
        } 
        case 2: {
            this->fUD_Directions[2] = myUltrasonicSensor.read();
            if(this->fUD_Directions[2] >= this->fUD_Directions[this->fUD_maxDistIndex]) this->fUD_maxDistIndex = 2;
            myServo.reset();
            if(!isDistInRange(this->fUD_Directions[this->fUD_maxDistIndex], 0, MIN_DISTANCE)) {
                if(this->fUD_maxDistIndex != 0) { // road ahead isn't clear
                    myEngine.turn90deg(this->fUD_maxDistIndex == 1);   //delay in turn90deg
                }
                this->isMovementBlocked = false;
                return;
            }
                    // if no way is free turn to the right or left at random
            uint8_t rand = random(0,2); // 0 do nothing, 1 turn right, 2 turn left
            myEngine.turn(rand == 1, SPEED_NORMAL);
            this->ts_blocked =millis();
            this->fUD_randomTurnTime=(random(200, 800));
            this->fUD_Index++;//case 3 -> waiting time
            return;
        }
        case 3:
            return;
        case 4:
            myEngine.stop();
            this->isMovementBlocked = false;
            return;
        default: { // default case should not happen
            this->isMovementBlocked = false; // would make another dist measure before starting again
        }
    }
}

/**
 * @brief checks if measure is in range
 * 
 */
boolean Tamagotchi::isDistInRange(unsigned int dist, unsigned int min, unsigned int max) {
    return (dist >= min) && (dist < max);
}

InstructionSet* Tamagotchi::randomInstructionSet() {
    uint8_t rand = random(0, 10);
    switch(rand) {
        case 0: return IS_FORWARD;
        case 1: return IS_SPIN_RIGHT;
        case 2: return IS_SPIN_LEFT;
        case 3: return IS_WIGGLE;
        default: return IS_FORWARD;
    }
}

/* @brief displays the face
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

void Tamagotchi::irReceive() {
    if(irRemote.DeviceDriverSet_IRrecv_Get(&irRecData) == false) {
        irRecData = 0;
    }
    #if DEBUG_IR_RECEIVE > 0 
        if(this->irRecData > 0) Serial.println((String) "IR: "+(this->irRecData));
    #endif
}

void Tamagotchi::irReceiveRoutine() {
    if(irRecData == 0) return;
    switch(irRecData) {
        case IR_1: // petting
            this->setIsFedFlag();
            break;
        case IR_2: 
            this->flag_is_pet = 1;
            break;
        case IR_OK: 
            // TODO clean load instruction set
            setInstructionSet(IS_STOP_60000);
            break;
        default: break;
    }
}

void Tamagotchi::setInstructionSet(InstructionSet *instrSet) {
    this->move_instructionSet = instrSet;
    this->isOrganicMovement = 1;
    this->move_instructionIndex = -1;
    this->move_instructionSetIndex = 1;
    this->ts_move_instruction = 0;
}