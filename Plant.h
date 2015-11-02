#ifndef __PLANT_H_INCLUDED__
#define __PLANT_H_INCLUDED__

#define TIME_TO_WAIT_UNTIL_NEXT_VALVE_OPEN 10000
#define MOISTURE_AVERAGE_INTERVAL 10000

#define PLANT_DEFAULT 0
#define PLANT_NEEDS_WATER 1

#include <Arduino.h>
#include "SolenoidValve.h"
#include <Time.h>
#include "Util.h"

class Plant {

  public:
    Plant(String name, int lowerMoistureThreshold, int upperMoistureThreshold, int moisturePin, int solenoidPin);
    int pollPlantSensor();
    void startWatering();
    bool isWatering() const;

  private:
    SolenoidValve *valve;
    bool _moistureThresholdHit;
    int _moisturePin;
    int _lowerMoistureThreshold;
    int _upperMoistureThreshold;
    int _currentMoisture;
    int _averageMoisture;
    unsigned long _localMoistureSum;
    int _localSumCounter;
    unsigned long _startTime;
    bool _watering;
    bool _isQueued;
    String _name;

    bool readPlantMoisture();
};

#endif
