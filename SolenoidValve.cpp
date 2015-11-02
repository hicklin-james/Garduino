#include "SolenoidValve.h"

SolenoidValve::SolenoidValve(int digitalPin) {
  valveState = STATE_CLOSED;
  _digitalPin = digitalPin;
  pinMode(digitalPin, OUTPUT);
}

void SolenoidValve::openValve() {
  digitalWrite(_digitalPin, HIGH);
  valveState = STATE_OPEN;
  startTime = millis();
}

int SolenoidValve::getSolenoidState() const {
  return valveState;
}

bool SolenoidValve::closeValveIfTimeElapsed() {
  if (valveState && ((millis() - startTime) > TIME_TO_LEAVE_VALVE_OPEN)) {
    closeValve();
    return true;
  }
  return false;
}

void SolenoidValve::closeValve() {
  digitalWrite(_digitalPin, LOW);
  valveState = STATE_CLOSED;
}