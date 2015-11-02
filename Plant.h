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
    Plant(String name, int upperMoistureThreshold, int lowerMoistureThreshold, int moisturePin, int solenoidPin);
    ~Plant();
    int pollPlantSensor();
    void startWatering();
    bool isWatering() const;

  private:
    SolenoidValve *valve; // solenoid valve corresponding to this plant
    bool _moistureThresholdHit; // boolean indicating the lower threshold was hit
    int _moisturePin; // pin that moisture sensor is on
    int _lowerMoistureThreshold; // lower moisture threshold --- LOW values mean more moisture
    int _upperMoistureThreshold; // upper moisture threshold --- HIGH values mean less moisture
    int _currentMoisture; // current moisture reading on sensor
    int _averageMoisture; // average moisture reading over MOISTURE_AVERAGE_INTERVAL reads
    unsigned long _localMoistureSum; // local moisture sum counter to calculate average
    int _localSumCounter; // local counter value to valvulate average
    unsigned long _startTime; // time that watering started
    bool _watering; // boolean indicating whether we are currently watering
    bool _isQueued; // boolean indicating whether we are queued to be watered
    String _name; // name of the plant, for print purposes

    bool readPlantMoisture();
};

#endif
