#include "ITR20001.h"

ITR20001 myITR;

boolean ITR20001::isTakenOffGround() {
    return (getM() >= THRESHOLD && getR() >= THRESHOLD && getL() >= THRESHOLD);
}

void ITR20001::init() {
    pinMode(ITR_PIN_L, INPUT);
    pinMode(ITR_PIN_M, INPUT);
    pinMode(ITR_PIN_R, INPUT);
}

int ITR20001::getL() {
    return analogRead(ITR_PIN_L);
}

int ITR20001::getM() {
    return analogRead(ITR_PIN_M);
}

int ITR20001::getR() {
    return analogRead(ITR_PIN_R);
}

