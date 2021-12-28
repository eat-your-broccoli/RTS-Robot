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
 * @param servo servo angle
 * @param time how long this instruction should be executed
 */
struct Instruction {
    uint8_t dir; // 0x00 forward, 0x11 back, 0x01 turn right, 0x10 turn left;
    uint8_t L;
    uint8_t R;
    uint8_t servo;
    uint16_t time;
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

extern InstructionSet *IS_FORWARD;
extern InstructionSet *IS_WIGGLE;
extern InstructionSet *IS_SPIN_RIGHT;
extern InstructionSet *IS_SPIN_LEFT;

