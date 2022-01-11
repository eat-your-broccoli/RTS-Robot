/**
 * interfacing with an FSLP to get pressure and position readings
 * https://github.com/pololu/fslp-led-strip-arduino-demo/tree/master/FslpLedStripDemo
 * 
 **/
#include "Arduino.h"

class FSLP {
    public:
        void init();
        int getPosition();
        int getPressure();
        boolean isTouch();
    private:
        void analogReset();
        #define PRESSURE_THRESHOLD 3500
        #define SL A8
        #define DL1 51
        #define DL2 A9
        #define DR 53
};

extern FSLP myFSLP;