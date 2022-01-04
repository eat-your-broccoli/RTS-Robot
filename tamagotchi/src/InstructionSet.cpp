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

InstructionSet** createIS_array(uint8_t length, uintptr_t instr[]){
    InstructionSet** is = (InstructionSet**) malloc(length * sizeof(InstructionSet));
    int i;
    for(i=0; i<length; i++) {
        is[i] = (InstructionSet*) instr[i];
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

const Instruction PROGMEM INS_STOP_5000 = {
    0b11,
    0,
    0,
    255,
    5000
};

const Instruction PROGMEM INS_STOP_2000 = {
    0b11,
    0,
    0,
    255,
    2000
};

const Instruction PROGMEM INS_STOP_1000 = {
    0b11,
    0,
    0,
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

const Instruction PROGMEM INS_TURN_LEFT_FAST_500 = {
    0b01,
    SPEED_FAST,
    SPEED_FAST,
    90,
    500 
};

const Instruction PROGMEM INS_TURN_LEFT_FAST_250 = {
    0b01,
    SPEED_FAST,
    SPEED_FAST,
    90,
    250 
};

const Instruction PROGMEM INS_TURN_LEFT_FAST_125 = {
    0b01,
    SPEED_FAST,
    SPEED_FAST,
    90,
    125 
};

const Instruction PROGMEM INS_TURN_LEFT_1000 = {
    0b01,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000 
};

const Instruction PROGMEM INS_TURN_LEFT_FAST_1000 = {
    0b01,
    SPEED_FAST,
    SPEED_FAST,
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

const Instruction PROGMEM INS_TURN_RIGHT_FAST_500 = {
    0b10,
    SPEED_FAST,
    SPEED_FAST,
    90,
    500 
};

const Instruction PROGMEM INS_TURN_RIGHT_FAST_250 = {
    0b10,
    SPEED_FAST,
    SPEED_FAST,
    90,
    250 
};

const Instruction PROGMEM INS_TURN_RIGHT_FAST_125 = {
    0b10,
    SPEED_FAST,
    SPEED_FAST,
    90,
    125 
};

const Instruction PROGMEM INS_TURN_RIGHT_1000 = {
    0b10,
    SPEED_NORMAL,
    SPEED_NORMAL,
    90,
    1000 
};

const Instruction PROGMEM INS_TURN_RIGHT_FAST_1000 = {
    0b10,
    SPEED_FAST,
    SPEED_FAST,
    90,
    1000 
};



InstructionSet *IS_STOP_60000 = createIS(true, 12, (Instruction[12]){
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
    INS_STOP_5000,
});

InstructionSet *IS_FORWARD = createIS(false, 2, (Instruction[2]){
    INS_FORWARD,
    INS_FORWARD    
});

InstructionSet *IS_SPIN_RIGHT = createIS(false, 4, (Instruction[4]){
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000    
});

InstructionSet *IS_SPIN_LEFT = createIS(false, 4, (Instruction[4]){
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000
});

InstructionSet *IS_SPIN_RIGHT_DELAYED = createIS(false, 6, (Instruction[6]){
    INS_STOP_5000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_STOP_1000   
});

InstructionSet *IS_SPIN_LEFT_DELAYED = createIS(false, 6, (Instruction[6]){
    INS_STOP_5000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_STOP_1000
});

InstructionSet *IS_WIGGLE = createIS(false, 9, (Instruction[9]){
    INS_TURN_LEFT_500,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_500,
    INS_STOP_1000
});

InstructionSet *IS_WIGGLE_DELAYED = createIS(false, 10, (Instruction[10]){
    INS_STOP_2000,
    INS_TURN_LEFT_500,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_1000,
    INS_TURN_LEFT_1000,
    INS_TURN_RIGHT_500,
    INS_STOP_1000
});

InstructionSet *IS_WIGGLE_DELAYED_FAST = createIS(false, 10, (Instruction[10]){
    INS_STOP_2000,
    INS_TURN_LEFT_FAST_500,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_RIGHT_FAST_1000,
    INS_TURN_LEFT_FAST_1000,
    INS_TURN_RIGHT_FAST_500,
    INS_STOP_1000,
});

InstructionSet *IS_WIGGLE_DELAYED_FAST_500 = createIS(false, 10, (Instruction[10]){
    INS_STOP_1000,
    INS_TURN_RIGHT_FAST_250,
    INS_TURN_LEFT_FAST_500,
    INS_TURN_RIGHT_FAST_500,
    INS_TURN_LEFT_FAST_500,
    INS_TURN_RIGHT_FAST_500,
    INS_TURN_LEFT_FAST_500,
    INS_TURN_RIGHT_FAST_500,
    INS_TURN_LEFT_FAST_500,
    INS_STOP_1000,
});

InstructionSet *IS_WIGGLE_DELAYED_FAST_250 = createIS(false, 10, (Instruction[10]){
    INS_STOP_1000,
    INS_TURN_LEFT_FAST_125,
    INS_TURN_RIGHT_FAST_250,
    INS_TURN_LEFT_FAST_250,
    INS_TURN_RIGHT_FAST_250,
    INS_TURN_LEFT_FAST_250,
    INS_TURN_RIGHT_FAST_250,
    INS_TURN_LEFT_FAST_250,
    INS_TURN_RIGHT_FAST_125,
    INS_STOP_1000,
});

InstructionSet* IS_ARRAY_PET[IS_ARRAY_PET_LENGTH] = {
    IS_SPIN_LEFT_DELAYED,
    IS_SPIN_RIGHT_DELAYED,
};

InstructionSet* IS_ARRAY_FED[IS_ARRAY_FED_LENGTH] = {
    // IS_WIGGLE_DELAYED,
    // IS_WIGGLE_DELAYED_FAST,
    IS_WIGGLE_DELAYED_FAST_500,
    IS_WIGGLE_DELAYED_FAST_250,
};

