#include "Arduino.h"

#define SPEED_SLOW 75
#define SPEED_NORMAL 120
#define SPEED_FAST 200

/**
 * @brief Instruction
 * this models a single instruction. Multiple instructions can be aggregated into a InstructionSet
 * e.g. multiple instructions of turning left and right can comprise a "wiggle" animation
 * 
 * @param dir the direction of each engine group moves
 * @param L speed of left engine group
 * @param R speed of right engine group
 * @param servo servo angle, set to 255 for random angle
 * @param time how long this instruction should be executed
 */
struct Instruction {
    uint8_t dir; // 0x00 forward, 0x11 back, 0x01 turn right, 0x10 turn left;
    uint8_t L;
    uint8_t R;
    uint8_t servo;
    uint16_t time;
    uint16_t randomTime;
};

/**
 * @brief InstructionSet
 * this models a group of instructions
 * 
 * @param blockDistMeasure should distance measure be blocked while executing this section (is useful e.g. for "wiggle")
 * @param length length of instr array
 * @param instr pointer of first instruction in array
 */
struct InstructionSet {
    bool blockDistMeasure = false;
    uint8_t length;
    Instruction** instr;
};

InstructionSet* createIS(bool blockDistMeasure, uint8_t length, Instruction (instr) []);
InstructionSet** createIS_array(uint8_t length, InstructionSet (instr) []);

extern InstructionSet *IS_FORWARD;
extern InstructionSet *IS_STOP_60000;
extern InstructionSet *IS_STOP_2000;
extern InstructionSet *IS_STOP_4000;


extern InstructionSet *IS_WIGGLE;
extern InstructionSet *IS_SPIN_RIGHT;
extern InstructionSet *IS_SPIN_LEFT;

#define IS_ARRAY_PET_LENGTH 2
extern InstructionSet *IS_ARRAY_PET[IS_ARRAY_PET_LENGTH];

#define IS_ARRAY_FED_LENGTH 3
extern InstructionSet *IS_ARRAY_FED[IS_ARRAY_FED_LENGTH];

#define IS_ARRAY_ORGANIC_LENGTH 46
extern InstructionSet *IS_ARRAY_ORGANIC[IS_ARRAY_ORGANIC_LENGTH];





