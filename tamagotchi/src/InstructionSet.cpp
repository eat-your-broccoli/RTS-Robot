#include "InstructionSet.h"

InstructionSet *createIS(bool blockDistMeasure, uint8_t length, Instruction instr[])
{
    InstructionSet *is = new InstructionSet;
    is->blockDistMeasure = blockDistMeasure;
    is->length = length;
    is->instr = (Instruction **)malloc(length * sizeof(Instruction));
    int i;
    for (i = 0; i < length; i++)
    {
        is->instr[i] = &instr[i];
    }
    return is;
}

InstructionSet **createIS_array(uint8_t length, uintptr_t instr[])
{
    InstructionSet **is = (InstructionSet **)malloc(length * sizeof(InstructionSet));
    int i;
    for (i = 0; i < length; i++)
    {
        is[i] = (InstructionSet *)instr[i];
    }
    return is;
}

const Instruction PROGMEM INS_FORWARD = {0b11, SPEED_NORMAL, SPEED_NORMAL, 90, 1000, 6000};

const Instruction PROGMEM INS_STOP_5000 = {0b11, 0, 0, 255, 5000, 0 };

const Instruction PROGMEM INS_STOP_2000 = {0b11, 0, 0, 255, 2000, 0 };

const Instruction PROGMEM INS_STOP_1000 = {0b11, 0, 0, 90, 1000, 0 };

const Instruction PROGMEM INS_BACKWARD = {0b00, SPEED_NORMAL, SPEED_NORMAL, 90, 1000, 0 };

const Instruction PROGMEM INS_TURN_LEFT_500 = {0b01, SPEED_NORMAL, SPEED_NORMAL, 90, 500, 0 };

const Instruction PROGMEM INS_TURN_LEFT_FAST_500 = {0b01, SPEED_FAST, SPEED_FAST, 90, 500, 0 };

const Instruction PROGMEM INS_TURN_LEFT_FAST_250 = {0b01, SPEED_FAST, SPEED_FAST, 90, 250, 0 };

const Instruction PROGMEM INS_TURN_LEFT_FAST_125 = {0b01, SPEED_FAST, SPEED_FAST, 90, 125, 0 };

const Instruction PROGMEM INS_TURN_LEFT_1000 = {0b01, SPEED_NORMAL, SPEED_NORMAL, 90, 1000, 0 };

const Instruction PROGMEM INS_TURN_LEFT_FAST_1000 = {0b01, SPEED_FAST, SPEED_FAST, 90, 1000, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_500 = {0b10, SPEED_NORMAL, SPEED_NORMAL, 90, 500, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_FAST_500 = {0b10, SPEED_FAST, SPEED_FAST, 90, 500, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_FAST_250 = {0b10, SPEED_FAST, SPEED_FAST, 90, 250, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_FAST_125 = {0b10, SPEED_FAST, SPEED_FAST, 90, 125, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_1000 = {0b10, SPEED_NORMAL, SPEED_NORMAL, 90, 1000, 0 };

const Instruction PROGMEM INS_TURN_RIGHT_FAST_1000 = {0b10, SPEED_FAST, SPEED_FAST, 90, 1000, 0 };

const Instruction PROGMEM INS_TURN_LEFT_SLOW_RAND = {0b01, SPEED_SLOW, SPEED_SLOW, 90, 200, 400 };

const Instruction PROGMEM INS_TURN_RIGHT_SLOW_RAND = {0b10, SPEED_SLOW, SPEED_SLOW, 90, 200, 400 };



// CURVED MOVEMENT
const Instruction PROGMEM FWD_LEFT_2 = {0b11, SPEED_NORMAL-2, SPEED_NORMAL, 92, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_5 = {0b11, SPEED_NORMAL-5, SPEED_NORMAL, 95, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_7 = {0b11, SPEED_NORMAL-7, SPEED_NORMAL, 97, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_10 = {0b11, SPEED_NORMAL-10, SPEED_NORMAL, 100, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_20 = {0b11, SPEED_NORMAL-20, SPEED_NORMAL, 110, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_30 = {0b11, SPEED_NORMAL-30, SPEED_NORMAL, 115, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_40 = {0b11, SPEED_NORMAL-40, SPEED_NORMAL, 120, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_50 = {0b11, SPEED_NORMAL - 50, SPEED_NORMAL, 125, 3000, 5000 };
const Instruction PROGMEM FWD_LEFT_60 = {0b11, SPEED_NORMAL - 60, SPEED_NORMAL, 130, 3000, 5000 };

const Instruction PROGMEM FWD_RIGHT_2 = {0b11, SPEED_NORMAL, SPEED_NORMAL-2, 88, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_5 = {0b11, SPEED_NORMAL, SPEED_NORMAL-5, 85, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_7 = {0b11, SPEED_NORMAL, SPEED_NORMAL-7, 83, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_10 = {0b11, SPEED_NORMAL, SPEED_NORMAL-10, 80, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_20 = {0b11, SPEED_NORMAL, SPEED_NORMAL-20, 70, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_30 = {0b11, SPEED_NORMAL, SPEED_NORMAL-30, 65, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_40 = {0b11, SPEED_NORMAL, SPEED_NORMAL-40, 60, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_50 = {0b11, SPEED_NORMAL, SPEED_NORMAL-50, 55, 3000, 5000 };
const Instruction PROGMEM FWD_RIGHT_60 = {0b11, SPEED_NORMAL, SPEED_NORMAL-60, 50, 3000, 5000 };


// const Instruction PROGMEM BWD_LEFT_2 = {0b00, SPEED_SLOW-2, SPEED_SLOW, 92, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_5 = {0b00, SPEED_SLOW-5, SPEED_SLOW, 95, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_7 = {0b00, SPEED_SLOW-7, SPEED_SLOW, 97, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_10 = {0b00, SPEED_SLOW-10, SPEED_SLOW, 100, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_20 = {0b00, SPEED_NORMAL-20, SPEED_NORMAL, 110, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_30 = {0b00, SPEED_NORMAL-30, SPEED_NORMAL, 115, 3000, 5000 };
// const Instruction PROGMEM BWD_LEFT_40 = {0b00, SPEED_NORMAL-40, SPEED_NORMAL, 120, 3000, 5000 };

// const Instruction PROGMEM BWD_RIGHT_2 = {0b00, SPEED_SLOW, SPEED_SLOW-2, 88, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_5 = {0b00, SPEED_SLOW, SPEED_SLOW-5, 85, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_7 = {0b00, SPEED_SLOW, SPEED_SLOW-7, 83, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_10 = {0b00, SPEED_SLOW, SPEED_SLOW-10, 80, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_20 = {0b00, SPEED_NORMAL, SPEED_NORMAL-20, 70, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_30 = {0b00, SPEED_NORMAL, SPEED_NORMAL-30, 65, 3000, 5000 };
// const Instruction PROGMEM BWD_RIGHT_40 = {0b00, SPEED_NORMAL, SPEED_NORMAL-40, 60, 3000, 5000 };


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

InstructionSet *IS_STOP_2000 = createIS(true, 1, (Instruction[1]){
                                                       INS_STOP_2000,
                                                   });

InstructionSet *IS_STOP_4000 = createIS(true, 2, (Instruction[2]){
                                                       INS_STOP_2000,
                                                       INS_STOP_2000                                                       
                                                   });
                                                   

InstructionSet *IS_FORWARD = createIS(false, 2, (Instruction[2]){INS_FORWARD, INS_FORWARD});

InstructionSet *IS_SPIN_RIGHT = createIS(true, 4, (Instruction[4]){INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000});

InstructionSet *IS_SPIN_LEFT = createIS(true, 4, (Instruction[4]){INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000});

InstructionSet *IS_SPIN_RIGHT_DELAYED = createIS(true, 6, (Instruction[6]){INS_STOP_2000, INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000, INS_TURN_RIGHT_FAST_1000, INS_STOP_1000});

InstructionSet *IS_SPIN_LEFT_DELAYED = createIS(true, 6, (Instruction[6]){INS_STOP_2000, INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000, INS_TURN_LEFT_FAST_1000, INS_STOP_1000});

InstructionSet *IS_WIGGLE = createIS(true, 9, (Instruction[9]){INS_TURN_LEFT_500, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_500, INS_STOP_1000});

InstructionSet *IS_WIGGLE_DELAYED = createIS(true, 10, (Instruction[10]){INS_STOP_2000, INS_TURN_LEFT_500, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_1000, INS_TURN_LEFT_1000, INS_TURN_RIGHT_500, INS_STOP_1000});

InstructionSet *IS_WIGGLE_DELAYED_FAST = createIS(true, 10, (Instruction[10]){
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

InstructionSet *IS_WIGGLE_DELAYED_FAST_500 = createIS(true, 10, (Instruction[10]){
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

InstructionSet *IS_WIGGLE_DELAYED_FAST_250 = createIS(true, 10, (Instruction[10]){
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

// InstructionSet * IS_MOVEMENT_FWD_L_1 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_2});
// InstructionSet * IS_MOVEMENT_FWD_L_2 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_5});
// InstructionSet * IS_MOVEMENT_FWD_L_3 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_7});
InstructionSet * IS_MOVEMENT_FWD_L_4 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_10});
InstructionSet * IS_MOVEMENT_FWD_L_5 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_20});
InstructionSet * IS_MOVEMENT_FWD_L_6 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_30});
InstructionSet * IS_MOVEMENT_FWD_L_7 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_40});
InstructionSet * IS_MOVEMENT_FWD_L_8 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_50});
InstructionSet * IS_MOVEMENT_FWD_L_9 = createIS(false, 1,  (Instruction[1]){ FWD_LEFT_60});


// InstructionSet * IS_MOVEMENT_BWD_L_1 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_2});
// InstructionSet * IS_MOVEMENT_BWD_L_2 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_5});
// InstructionSet * IS_MOVEMENT_BWD_L_3 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_7});
// InstructionSet * IS_MOVEMENT_BWD_L_4 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_10});
// InstructionSet * IS_MOVEMENT_BWD_L_5 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_20});
// InstructionSet * IS_MOVEMENT_BWD_L_6 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_30});
// InstructionSet * IS_MOVEMENT_BWD_L_7 = createIS(false, 1,  (Instruction[1]){ BWD_LEFT_40});

// InstructionSet * IS_MOVEMENT_FWD_R_1 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_2});
// InstructionSet * IS_MOVEMENT_FWD_R_2 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_5});
// InstructionSet * IS_MOVEMENT_FWD_R_3 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_7});
InstructionSet * IS_MOVEMENT_FWD_R_4 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_10});
InstructionSet * IS_MOVEMENT_FWD_R_5 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_20});
InstructionSet * IS_MOVEMENT_FWD_R_6 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_30});
InstructionSet * IS_MOVEMENT_FWD_R_7 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_40});
InstructionSet * IS_MOVEMENT_FWD_R_8 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_50});
InstructionSet * IS_MOVEMENT_FWD_R_9 = createIS(false, 1,  (Instruction[1]){ FWD_RIGHT_60});


// InstructionSet * IS_MOVEMENT_BWD_R_1 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_2});
// InstructionSet * IS_MOVEMENT_BWD_R_2 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_5});
// InstructionSet * IS_MOVEMENT_BWD_R_3 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_7});
// InstructionSet * IS_MOVEMENT_BWD_R_4 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_10});
// InstructionSet * IS_MOVEMENT_BWD_R_5 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_20});
// InstructionSet * IS_MOVEMENT_BWD_R_6 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_30});
// InstructionSet * IS_MOVEMENT_BWD_R_7 = createIS(false, 1,  (Instruction[1]){ BWD_RIGHT_40});

InstructionSet * IS_TURN_LEFT = createIS(false, 1,  (Instruction[1]){ INS_TURN_LEFT_SLOW_RAND});
InstructionSet * IS_TURN_RIGHT = createIS(false, 1,  (Instruction[1]){ INS_TURN_RIGHT_SLOW_RAND});

InstructionSet *IS_ARRAY_PET[IS_ARRAY_PET_LENGTH] = {
    IS_SPIN_LEFT_DELAYED,
    IS_SPIN_RIGHT_DELAYED,
};

InstructionSet *IS_ARRAY_FED[IS_ARRAY_FED_LENGTH] = {
    IS_WIGGLE_DELAYED,
    IS_WIGGLE_DELAYED_FAST_500,
    IS_WIGGLE_DELAYED_FAST_250,
};

InstructionSet *IS_ARRAY_ORGANIC[IS_ARRAY_ORGANIC_LENGTH] = {
    IS_STOP_60000,
    
    IS_STOP_2000,
    IS_STOP_2000,
    IS_STOP_4000,
    IS_STOP_4000,

    IS_FORWARD,
    IS_FORWARD,
    IS_FORWARD,
    IS_FORWARD,

    IS_TURN_LEFT,
    IS_TURN_RIGHT,

    // IS_MOVEMENT_FWD_L_1,
    // IS_MOVEMENT_FWD_L_2,
    // IS_MOVEMENT_FWD_L_3,
    IS_MOVEMENT_FWD_L_4,
    IS_MOVEMENT_FWD_L_5,
    IS_MOVEMENT_FWD_L_6,
    IS_MOVEMENT_FWD_L_7,
    IS_MOVEMENT_FWD_L_8,
    IS_MOVEMENT_FWD_L_9,
    
    
    // IS_MOVEMENT_BWD_L_1,
    // IS_MOVEMENT_BWD_L_2,
    // IS_MOVEMENT_BWD_L_3,
    // IS_MOVEMENT_BWD_L_4,
    // IS_MOVEMENT_BWD_L_5,
    // IS_MOVEMENT_BWD_L_6,
    // IS_MOVEMENT_BWD_L_7,

    // IS_MOVEMENT_FWD_R_1,
    // IS_MOVEMENT_FWD_R_2,
    // IS_MOVEMENT_FWD_R_3,
    IS_MOVEMENT_FWD_R_4,
    IS_MOVEMENT_FWD_R_5,
    IS_MOVEMENT_FWD_R_6,
    IS_MOVEMENT_FWD_R_7,
    IS_MOVEMENT_FWD_R_8,
    IS_MOVEMENT_FWD_R_9,
    
    
    // IS_MOVEMENT_BWD_R_1,
    // IS_MOVEMENT_BWD_R_2,
    // IS_MOVEMENT_BWD_R_3,
    // IS_MOVEMENT_BWD_R_4,
    // IS_MOVEMENT_BWD_R_5,
    // IS_MOVEMENT_BWD_R_6,
    // IS_MOVEMENT_BWD_R_7,
};