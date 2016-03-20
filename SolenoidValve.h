#ifndef __SOLENOID_VALVE_H_INCLUDED__
#define __SOLENOID_VALVE_INCLUDED__

#define STATE_CLOSED 0
#define STATE_OPEN 1
#define TIME_TO_LEAVE_VALVE_OPEN 6000

#include <Arduino.h>
#include <Time.h>
#include "Util.h"

class SolenoidValve {

  public:
    SolenoidValve(int digitalPin);
    void openValve();
    bool closeValveIfTimeElapsed();
    int getSolenoidState() const;

  private:
    int valveState; // current state of valve -- either STATE_OPEN or STATE_CLOSED
    unsigned long startTime; // time that valve was opened
    int _digitalPin; // digital pin number

    void closeValve();

};

#endif
