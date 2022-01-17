#include "Arduino.h"

/**
 * @brief controls ITR20001 line tracking device
 * 
 */
class ITR20001 {
    public: 
        void init();
        boolean isTakenOffGround();
        int getL();
        int getM();
        int getR();
    private: 
        #define THRESHOLD 900
        #define ITR_PIN_R A0
        #define ITR_PIN_M A1
        #define ITR_PIN_L A2
};

extern ITR20001 myITR;