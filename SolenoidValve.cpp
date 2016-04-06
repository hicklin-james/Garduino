#include "SolenoidValve.h"

/**
  Constructor
  Description:
    Initializes private vars and sets the arduino digital pin as an output
**/
SolenoidValve::SolenoidValve(int digitalPin) {
  valveState = STATE_CLOSED;
  _digitalPin = digitalPin;
  pinMode(digitalPin, OUTPUT);
}

/**
  Returns: void
  Description:
    Opens the valve and resets the startTime to the current time
**/
void SolenoidValve::openValve() {
  //Util::print("Opening valve: %d\n", _digitalPin);
  digitalWrite(_digitalPin, HIGH);
  valveState = STATE_OPEN;
  startTime = millis();
}

/**
  Returns: int
    STATE_CLOSED -> valve is currently closed
    STATE_OPEN -> valve is currently open
  Description:
    Get the current state of the solenoid
**/
int SolenoidValve::getSolenoidState() const {
  return valveState;
}

/**
  Returns: bool
    true -> valve was closed
    false -> valve was not closed
  Description:
    Determines whether the solenoid needs to be closed, and if so, calls
    the corresponding function to close it
**/
bool SolenoidValve::closeValveIfTimeElapsed() {
  if (valveState && ((millis() - startTime) > TIME_TO_LEAVE_VALVE_OPEN)) {
    closeValve();
    return true;
  }
  return false;
}

/**
  Returns: void
  Description:
    Closes the solenoid
**/
void SolenoidValve::closeValve() {
  digitalWrite(_digitalPin, LOW);
  valveState = STATE_CLOSED;
}


