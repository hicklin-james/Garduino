#ifndef __PLANT_H_INCLUDED__
#define __PLANT_H_INCLUDED__

#define TIME_TO_WAIT_UNTIL_NEXT_VALVE_OPEN 600000
#define MOISTURE_AVERAGE_INTERVAL 10000
#define TIME_BETWEEN_MOISTURE_READINGS 3600000
//#define TIME_BETWEEN_MOISTURE_READINGS 10000
#define ANALOG_SENSOR_TIME_TO_INIT 200

#define PLANT_DEFAULT 0
#define PLANT_NEEDS_WATER 1

#define MS_OF_LIGHT 43200000
#define MS_IN_DAY 86400000

#include <Arduino.h>
#include "SolenoidValve.h"
#include <Time.h>
#include "Util.h"

class Plant {

  public:
    Plant(String name, int upperMoistureThreshold, int lowerMoistureThreshold, int moisturePin, int moisturePowerPin, int growLightPin, int solenoidPin);
    ~Plant();
    int pollPlantSensor();
    int evaluateGrowLights();
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
    int _localSumCounter; // local counter value to calculate average
    unsigned long _startTime; // time that watering started
    bool _watering; // boolean indicating whether we are currently watering
    bool _isQueued; // boolean indicating whether we are queued to be watered
    String _name; // name of the plant, for print purposes
    unsigned long _lastMoistureReading; // last time that moisture was measured
    int _isIdle;
    unsigned long _analogSensorInitTime;
    int _moisturePowerPin;
    int _growLightPin;
    unsigned long _timeLightTurnedOn;
    unsigned long _timeLightTurnedOff;
    unsigned long _lastNow;
    bool _isLit;
    int _lastHour;
    int _lastDay;
    bool _firstReadSincePowered;

    void initAnalogSensor();
    bool analogSensorReady();
    bool readPlantMoisture();
};

#endif
