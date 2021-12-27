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

// organic movement 
#define MOVE_ACTION_COOLDOWN 3000 // 3000 ms = 3sec
#define MOVE_UNBLOCK_COOLDOWN 2500 // 1000ms = 1sec


#define MIN_DISTANCE 20

#define SPEED_SLOW 75
#define SPEED_NORMAL 120
#define SPEED_FAST 200
/**
 * @brief Tamagotchi class
 * 
 * Controls the robot in a pet-like manner
 * includes sentiment like hunger, sleepyness and affection
 * 
 */


Tamagotchi myTamagotchi;
//DeviceDriverSet_ULTRASONIC AppULTRASONIC;

Tamagotchi::Tamagotchi() {
    this->sleepyness = 50;
    this->hunger = 50;
    this->affection = 20;
};

void Tamagotchi::init() {
    Serial.println("reading data from EEPROM...");
    readDataFromEEPROM();
    Serial.println("done reading data from EEPROM");

    myUltrasonicSensor.init();
    myEngine.init();
    myServo.init();

    myServo.turn(SERVO_CENTER_RIGHT);
    delay(500);
    myServo.turn(SERVO_CENTER_LEFT);
    delay(500);
    myServo.reset();
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

    organicMovement();
    
    
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

/**
 * @brief handles organic movement
 * 
 * 
 * what is a instruction set? a instruction consist of an action 
 * like moving, turning, ... and a timespan during which the instruction has to be done
 */
void Tamagotchi::organicMovement() {
    unsigned long time = millis();
    // check if last other action was at least n seconds in the past
    if(this->isOrganicMovement == false &&
     (time < this->ts_move_cooldown || (time - this->ts_move_cooldown) < MOVE_ACTION_COOLDOWN)) {
        // wait until enough time has passed
        return;
    }
    // check if obstacle is present
    // if yes, stop. Turn a bit
    uint16_t dist = myUltrasonicSensor.read();
    // if ultrasonic sensor detects obstacle within 20cm of range
    // Serial.println((String) "my distance is "+dist);
    if(isDistInRange(dist, 0, MIN_DISTANCE)) {
        // Serial.println("movement is blocked");
        findUnblockedDirection();
        myServo.reset();
        return;
    }
    this->isMovementBlocked = false;
    this->isOrganicMovement = 0;

    // check if not already moving organically
    if(this->isOrganicMovement == 0) {
        // reset instruction set if it was not properly resetted
        this->move_instructionSet = -1;
        this->ts_move_instruction = 0;
        this->isOrganicMovement = 1;
    }

    if(time - this->ts_move_instruction < 3000) {
        // do nothing
        // TODO hardcoded limit that each instruction should be done for 3 sec
        return;
    }

    if(this->move_instructionSet == -1) {
        // TODO 
        // check is instruction set is loaded

        // if not, choose random instruction set 
        this->move_instructionSet = 1;
    }

    // execute instruction set
    if(this->move_instructionSet == 1) {
        // TODO hardcoded for one instruction
        myEngine.move(1, SPEED_NORMAL, 1, SPEED_NORMAL);
        this->ts_move_instruction = time;
    }
}

void Tamagotchi::stopOrganicMovement() {
    myEngine.stop();
    this->isOrganicMovement = 0;
    this->move_instructionIndex = 0;
    this->move_instructionSet = -1;
}

/**
 * @brief turns around and finds a direction that is not blocked by an obstacle
 * 
 */
// void Tamagotchi::findUnblockedDirection2() {
//     unsigned long time = millis();
//     uint16_t dist = 0;

//     // if previously the movement wasn't blocked, stop the engine to avoid hitting obstacle
//     if(this->isMovementBlocked == false) {
//         Serial.println("movement is now blocked");
//         myEngine.stop();
//         this->ts_move_instruction = 0;
//         this->isMovementBlocked = true;
//         this->ts_blocked = 0; 
//         this->blocked_instructionIndex = 0;
//     }

//     // if enough time has passed, we do the next instruction
//     if(this->ts_blocked == 0 || time < this->ts_blocked || time - this->ts_blocked > MOVE_UNBLOCK_COOLDOWN) {
//         Serial.println("next step in blocked");
//         this->blocked_instructionIndex++;
//         this->blocked_instructionIndex
//         this->ts_blocked = time;
//     } else {
//         return;
//     }

//     switch(this->blocked_instructionIndex) {
//         case 1: // look left 
//             Serial.println("looking left");
//             myServo.turn(SERVO_CENTER_LEFT);
//             break;
//         case 2: // take measurement
//             dist = myUltrasonicSensor.read();
//             break;
//         case 3: // look right
//             Serial.println("looking right");
//             myServo.turn(SERVO_CENTER_RIGHT);
//             break;
//         case 4: // take measurement
//             dist = myUltrasonicSensor.read();
//             break;
//     }

//     Serial.print("dist: "); Serial.println(dist);
//     if(dist >= 0 && dist <= MIN_DISTANCE) {
//         // path still blocked
//         return;
//     }
//     myServo.turn(90);
//     this->isMovementBlocked = false;

//     // if(this->blocked_instructionIndex == 2) {
//     //     Serial.println
//     //     myEngine.turn(false, 90);
//     // } else {
//     //     myEngine.turn(true, 90);       
//     // }
// }

/**
 * @brief turns around and finds a direction that is not blocked by an obstacle
 * 
 */
void Tamagotchi::findUnblockedDirection() {
    myEngine.stop();
    uint16_t dist[3];
    unsigned int maxDistIndex;

    // if previously the movement wasn't blocked, stop the engine to avoid hitting obstacle
    if(this->isMovementBlocked == false) {
        Serial.println("movement is now blocked");
        this->ts_move_instruction = 0;
        this->isMovementBlocked = true;
        this->ts_blocked = 0; 
        this->blocked_instructionIndex = 0;
    }

    delay(200);
    dist[0] = myUltrasonicSensor.read();
    myServo.turn(SERVO_CENTER_RIGHT);
    delay(200);
    dist[1] = myUltrasonicSensor.read();
    if(dist[1] >= dist[0]) maxDistIndex = 1;
    myServo.turn(SERVO_CENTER_LEFT);
    delay(200);
    dist[2] = myUltrasonicSensor.read();
    if(dist[2] >= dist[maxDistIndex]) maxDistIndex = 2;
    myServo.reset();
    if(!isDistInRange(dist[maxDistIndex], 0, MIN_DISTANCE)) {
        if(maxDistIndex != 0) { // road ahead isn't clear
            myEngine.turn90deg(maxDistIndex == 1);  
        }
        this->isMovementBlocked = false;
        return;
    }
    delay(200);
    myEngine.backward(SPEED_NORMAL);
    delay(300);
    myEngine.stop();
}

boolean Tamagotchi::isDistInRange(unsigned int dist, unsigned int min, unsigned int max) {
    return (dist >= min) && (dist < max);
}