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
<<<<<<< HEAD
        #define PRESSURE_THRESHOLD 200
=======
        #define PRESSURE_THRESHOLD 100
>>>>>>> master
        #define SL A8
        #define DL1 51
        #define DL2 A9
        #define DR 53
};

extern FSLP myFSLP;