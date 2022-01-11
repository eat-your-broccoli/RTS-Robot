#include "FSLP.h"

FSLP myFSLP;

void FSLP::init() {

}

int FSLP::getPosition() {
  // Step 1 - Clear the charge on the sensor.
  pinMode(SL, OUTPUT);
  digitalWrite(SL, LOW);

  pinMode(DL1, OUTPUT);
  digitalWrite(DL1, LOW);

  pinMode(DL2, OUTPUT);
  digitalWrite(DL2, LOW);

  pinMode(DR, OUTPUT);
  digitalWrite(DR, LOW);

  // Step 2 - Set up appropriate drive line voltages.
  digitalWrite(DL1, HIGH);
  pinMode(DR, INPUT);
  pinMode(SL, INPUT);

  // Step 3 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 4 - Take the measurement.
  analogReset();
  return analogRead(SL);
}

int FSLP::getPressure() {
  // Step 1 - Set up the appropriate drive line voltages.
  pinMode(DL1, OUTPUT);
  digitalWrite(DL1, HIGH);

  pinMode(DR, OUTPUT);
  digitalWrite(DR, LOW);

  pinMode(SL, INPUT);

  pinMode(DL2, INPUT);

  // Step 2 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 3 - Take two measurements.
  analogReset();
  int v1 = analogRead(DL2);
  analogReset();
  int v2 = analogRead(SL);

  // Step 4 - Calculate the pressure.
  // Detailed information about this formula can be found in the
  // FSLP Integration Guide.
  if (v1 == v2)
  {
    // Avoid dividing by zero, and return maximum reading.
    return 32 * 1023;
  }
  return 32 * v2 / (v1 - v2);
}

void FSLP::analogReset() {
    #if defined(ADMUX)
    #if defined(ADCSRB) && defined(MUX5)
        // Code for the ATmega2560 and ATmega32U4
        ADCSRB |= (1 << MUX5);
    #endif
        ADMUX = 0x1F;

        // Start the conversion and wait for it to finish.
        ADCSRA |= (1 << ADSC);
        loop_until_bit_is_clear(ADCSRA, ADSC);
    #endif
}

boolean FSLP::isTouch() {
    int pressure = myFSLP.getPressure();
    if(pressure >= PRESSURE_THRESHOLD) {
        return true;
    }
    return false;
    // int position = myFSLP.getPosition();
}