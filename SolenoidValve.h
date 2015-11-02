#ifndef __SOLENOID_VALVE_H_INCLUDED__
#define __SOLENOID_VALVE_INCLUDED__

#define STATE_CLOSED 0
#define STATE_OPEN 1
#define TIME_TO_LEAVE_VALVE_OPEN 2000

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
    int valveState;
    unsigned long startTime;
    int _digitalPin;

    void closeValve();

};

#endif
