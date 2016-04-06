#include "Plant.h"

/**
  Constructor
  Description:
    Initializes private variables and reads a sensor value
**/
Plant::Plant(String name, int upperMoistureThreshold, int lowerMoistureThreshold, int moisturePin, int moisturePowerPin, int growLightPin, int solenoidPin) {
  _name = name;
  _isQueued = false;
  valve = new SolenoidValve(solenoidPin);
  _lowerMoistureThreshold = lowerMoistureThreshold;
  _upperMoistureThreshold = upperMoistureThreshold;
  _moisturePin = moisturePin;
  _moistureThresholdHit = false;
  _localSumCounter = 0;
  _watering = false;
  _lastMoistureReading = millis() - TIME_BETWEEN_MOISTURE_READINGS;
  _moisturePowerPin = moisturePowerPin;
  _isLit = 0;
  _timeLightTurnedOn = 0;
  _timeLightTurnedOff = 0;
  _lastHour = hour();
  _lastDay = day();
  _lastNow = 0;
  _growLightPin = growLightPin;
  _firstReadSincePowered = true;
  pinMode(moisturePowerPin, OUTPUT);
  pinMode(growLightPin, OUTPUT);
  initAnalogSensor();
}

/**
  Destructor
  Description:
    Deallocs the valve
**/
Plant::~Plant() {
  delete valve;
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

int Plant::evaluateGrowLights() {
  unsigned long now = millis();
  if (_growLightPin >= 0) {
    if (now != 0) {
      //Util::print("Time light turned on: %d\n", _timeLightTurnedOn);
      //Util::print("now: %d\n", now);
      if (_timeLightTurnedOff != 0 && ((now - _timeLightTurnedOff) < (MS_IN_DAY - MS_OF_LIGHT))) {
        //Util::print("%d\n", MS_IN_DAY - MS_OF_LIGHT);
        return 0;
      }
      else if (_isLit == 1 && _timeLightTurnedOn != 0 && ((now - _timeLightTurnedOn) > MS_OF_LIGHT)) {
        // turn the light off
        //Util::print("Turning Light Off\n");
        //Util::print("pin: %d", _growLightPin);
        digitalWrite(_growLightPin, LOW);
        _timeLightTurnedOff = now;
        _isLit = 0;
      }
      else {
        // turn the light on
        if (_isLit == 0) {
          //Util::print("Turning Light On\n");
          digitalWrite(_growLightPin, HIGH);
          _timeLightTurnedOn = now;
          //Util::print("TimeLightTurnedOn: %u\n", _timeLightTurnedOn);
          _isLit = 1;
        }
      }
    }
    _lastNow = now;
  }
}

/**
  Returns: int
    PLANT_DEFAULT -> no action to be taken
    PLANT_NEEDS_WATER -> the plant needs to be watered
  Description:
    Main plant logic function used to determine what action needs to be taken on the plant,
    if any.
**/
int Plant::pollPlantSensor(RequestManager reqManager) {
  if (readPlantMoisture()) {
    String reqString = "/sensor_readings/add_reading?plant_name=" + _name + "&value=" + _medianMoisture;
    //reqManager.get("611f2ca7.ngrok.io", "/boo?test=test", 80);
    //Serial.println(reqString);
    reqManager.get("garduino.jameshicklin.com", reqString.c_str(), 80);
    //char str[ ] = "abbf535e.ngrok.io";
    //Serial.println(str);
    //reqManager.get("abbf535e.ngrok.io", "/boo\?test=test", 80);
    // if we haven't returned already,

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
      // if the current moisture level is now below the lower moisture threshold,
      // we can just reset the moisture threshold bool and leave the valve closed
      else if ((_medianMoisture < _lowerMoistureThreshold) && _moistureThresholdHit) {
        _moistureThresholdHit = false;
        digitalWrite(_moisturePowerPin, LOW);   
        _isIdle = true;
        return PLANT_DEFAULT;
      }
      // if the moisture threshold was previously hit, we are still above the lower
      // moisture threshold, and enough time has elapsed since we last opened the solenoid valve,
      // then we should open the valve again
      else if ((_medianMoisture > _lowerMoistureThreshold) && _moistureThresholdHit &&
          (millis() - _startTime) > TIME_TO_WAIT_UNTIL_NEXT_VALVE_OPEN) {
        _isQueued = true;
        return PLANT_NEEDS_WATER;
      }

      // if we are above the upper moisture threshold, open the valve and set the bool
      // indicating that we have hit the moisture threshold.
      else if (_medianMoisture > _upperMoistureThreshold && !_moistureThresholdHit) {
        _moistureThresholdHit = true;
        _isQueued = true;
        return PLANT_NEEDS_WATER;
      }

      else if ((_medianMoisture < _lowerMoistureThreshold) && !_moistureThresholdHit) {
        digitalWrite(_moisturePowerPin, LOW);   
        _isIdle = true;
        return PLANT_DEFAULT;
      }
    }
  }
  return PLANT_DEFAULT;
}

/**
  Returns: bool
    true -> a new median value has been calculated
    false -> no new median has been calculated
  Description:
    This function reads the current analog value from the moisture sensor.
    If the local counter is at the predefined interval, calculate a median
    value for the moisture and store it in _medianMoisture. The reason we 
    use the median is to prevent random variations in the value from triggering
    the plant watering when it doesn't need to be watered.
**/
bool Plant::readPlantMoisture() {
  if (((millis() - _lastMoistureReading) > TIME_BETWEEN_MOISTURE_READINGS) && _isIdle) {
    initAnalogSensor();
  }
  if (!_isIdle) {
    _currentMoisture = analogRead(_moisturePin);
    _moistureReadings[_localSumCounter] = _currentMoisture;
   // _localMoistureSum += _currentMoisture;
    if (_localSumCounter == (MOISTURE_AVERAGE_INTERVAL - 1)) {
      Util::quickSort(_moistureReadings, 0, MOISTURE_AVERAGE_INTERVAL-1);
      _medianMoisture = _moistureReadings[MOISTURE_AVERAGE_INTERVAL / 2];
      //_averageMoisture = _localMoistureSum / MOISTURE_AVERAGE_INTERVAL;
      _localSumCounter = 0;
      //_localMoistureSum = 0;
      if (_firstReadSincePowered) {
        _firstReadSincePowered = false;
        return false;
      }
      else {
        return true;
      }
    }
    _localSumCounter++;
    return false;
  }
  else {
    return false;
  }
}

bool Plant::analogSensorReady() {
  return ((millis() - _analogSensorInitTime) > ANALOG_SENSOR_TIME_TO_INIT);
}

void Plant::initAnalogSensor() {
  digitalWrite(_moisturePowerPin, HIGH);
  _analogSensorInitTime = millis();
  _lastMoistureReading = millis();
  _isIdle = false;
  _firstReadSincePowered = true;
}


