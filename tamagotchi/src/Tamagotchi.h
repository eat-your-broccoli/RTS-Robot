#include <avr/interrupt.h>

class Tamagotchi {
    private:
    int affection;
    int hunger;
    int sleepyness;

    volatile char flag_save;
    volatile char flag_is_pet;
    volatile char flag_is_fed;
    volatile char flag_read_battery;

    public:
    void init();
    void loop();

    private:
    
};