#include "Plant.h"

Plant::Plant(String name, int lowerMoistureThreshold, int upperMoistureThreshold, int moisturePin, int solenoidPin) {
  _name = name;
  _isQueued = false;
  valve = new SolenoidValve(solenoidPin);
  _lowerMoistureThreshold = lowerMoistureThreshold;
  _upperMoistureThreshold = upperMoistureThreshold;
  _moisturePin = moisturePin;
  _moistureThresholdHit = false;
  _localSumCounter = 1;
  _watering = false;
  readPlantMoistureIntoCurrentMoisture();
}

/**
  Returns: void
  Description:
    Starts watering the plant.
**/
void Plant::startWatering() {
  valve->openValve();
  _watering = true;
  _isQueued = false;
  _startTime = millis();
}

/**
  Returns: bool
    true -> the plant is currently being watered
    false -> the plant is not currently being watered
  Description:
    Used to determine whether the plant is currently being watered or not.
**/
bool Plant::isWatering() const {
  return _watering;
}

/**
  Returns: int
    PLANT_DEFAULT -> no action to be taken
    PLANT_NEEDS_WATER -> the plant needs to be watered
  Description:
    Main plant logic function used to determine what action needs to be taken on the plant,
    if any.
**/
int Plant::pollPlantSensor() {
  if (readPlantMoisture()) {
    // if the valve is already open
    if (valve->getSolenoidState()) {
      // close the valve if the necessary amount of time has elapsed
      ///Util::print("Valve is open, so checking whether it should be closed\n");
      if (valve->closeValveIfTimeElapsed()) {
        _watering = false;
      }
      return PLANT_DEFAULT;
    }
    // otherwise check to see if we need to open the valve
    else {
      // if it's watering or queued, don't do anything and return
      if (_watering || _isQueued) {
        return PLANT_DEFAULT;
      }
      // if the current moisture level is now above the upper moisture threshold,
      // we can just reset the moisture threshold bool and leave the valve closed
      else if (_averageMoisture < _upperMoistureThreshold) {
        _moistureThresholdHit = false;
        return PLANT_DEFAULT;
      }
      // if the moisture threshold was previously hit, we are still below the upper 
      // moisture threshold, and enough time has elapsed since we last opened the solenoid valve,
      // then we should open the valve again
      else if ((_averageMoisture > _upperMoistureThreshold) && _moistureThresholdHit &&
          (millis() - _startTime) > TIME_TO_WAIT_UNTIL_NEXT_VALVE_OPEN) {
        _isQueued = true;
        return PLANT_NEEDS_WATER;
      }

      // if we are below the lower moisture threshold, open the valve and set the bool
      // indicating that we have hit the moisture threshold.
      else if (_averageMoisture > _lowerMoistureThreshold && !_moistureThresholdHit) {
        _moistureThresholdHit = true;
        _isQueued = true;
        return PLANT_NEEDS_WATER;
      }
    }
  }
  return PLANT_DEFAULT;
}

/**
  Returns: bool
    true -> a new average value has been calculated
    false -> no new average has been calculated
  Description:
    This function reads the current analog value from the moisture sensor.
    If the local counter is at the predefined interval, calculate an average
    value for the moisture and store it in _averageMoisture. The reason we 
    average it is to prevent random variations in the value from triggering
    the plant watering when it doesn't need to be.
**/
bool Plant::readPlantMoisture() {
  _currentMoisture = analogRead(_moisturePin);
  _localMoistureSum += _currentMoisture;
  if (_localSumCounter == MOISTURE_AVERAGE_INTERVAL) {
    _averageMoisture = _localMoistureSum / MOISTURE_AVERAGE_INTERVAL;
    _localSumCounter = 1;
    _localMoistureSum = 0;
    return true;
  }
  _localSumCounter++;
  return false;
}
