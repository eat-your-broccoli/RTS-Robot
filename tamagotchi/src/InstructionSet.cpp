#include "InstructionSet.h"

InstructionSet* createIS(bool blockDistMeasure, uint8_t length, Instruction instr[]) {
    InstructionSet *is = new InstructionSet;
    is->blockDistMeasure = blockDistMeasure;
    is->length = length;
    is->instr = (Instruction**) malloc(length * sizeof(Instruction));
    int i;
    for(i=0; i<length; i++) {
        is->instr[i] = &instr[i];
    }
    return is;
}

const Instruction PROGMEM INS_FORWARD = {
    0b11,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000
};

const Instruction PROGMEM INS_BACKWARD = {
    0b00,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000 
};

const Instruction PROGMEM INS_TURN_LEFT_500 = {
    0b01,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    500 
};

const Instruction PROGMEM INS_TURN_LEFT_1000 = {
    0b01,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000 
};

const Instruction PROGMEM INS_TURN_RIGHT_500 = {
    0b10,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    500 
};

const Instruction PROGMEM INS_TURN_RIGHT_1000 = {
    0b10,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000 
};

InstructionSet *IS_FORWARD = createIS(false, 2, (Instruction[2]){
    INS_FORWARD,
    INS_FORWARD    
});

InstructionSet *IS_SPIN_RIGHT = createIS(false, 2, (Instruction[2]){
    INS_TURN_RIGHT_1000,
    INS_TURN_RIGHT_1000    
});

InstructionSet *IS_SPIN_LEFT = createIS(false, 2, (Instruction[2]){
    INS_TURN_LEFT_1000,
    INS_TURN_LEFT_1000    
});

InstructionSet *IS_WIGGLE = createIS(false, 8, (Instruction[8]){
    INS_TURN_LEFT_500,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_500
});
