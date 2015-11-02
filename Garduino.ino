// --------------------------------------------------------------------------------------- //
// for some stupid reason, arduino needs you to include the library header in the main ino
// file before you can include it in linked h files. 
#include <Time.h>
#include <QueueList.h>
// --------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------- //
// Main includes
#include "Plant.h"
// --------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------- //
// Constants
#define NUM_PLANTS 2
#define MAX_PLANTS 10

struct PlantAtts {
  String name;
  int solenoidPin;
  int moisturePin;
  int lowerMoistureThreshold;
  int upperMoistureThreshold;
};

struct PlantAtts plant1 = {
  "Basil",
  2,
  0,
  900,
  800
};

struct PlantAtts plant2 = {
  "Rosemary",
  2,
  0,
  900,
  800
};

struct PlantAtts initialPlantAtts[NUM_PLANTS] = {plant1, plant2};
// --------------------------------------------------------------------------------------- //

Plant *plants[MAX_PLANTS];
QueueList<Plant*> plantsToWaterQueue;
Plant *currentlyWatering;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < NUM_PLANTS; i++) {
    struct PlantAtts plantAtts = initialPlantAtts[i];
    Plant *plant = new Plant(plantAtts.name, plantAtts.lowerMoistureThreshold, plantAtts.upperMoistureThreshold, plantAtts.moisturePin, plantAtts.solenoidPin);
    plants[i] = plant;
  }
  currentlyWatering = NULL;
}

void loop() {
  // if there are any plants in the queue waiting to be watered, and no plant is currently
  // being watered, pop and water.
  if (!plantsToWaterQueue.isEmpty() && (currentlyWatering == NULL || !currentlyWatering->isWatering())) {
    currentlyWatering = plantsToWaterQueue.pop();
    currentlyWatering->startWatering();
  }


  for (int i = 0; i < NUM_PLANTS; i++) {
    Plant *plant = plants[i];
    int plantStatus = plant->pollPlantSensor();
    // If the plant indicates that it needs to be watered, add it to the plantsToWater queue
    if (plantStatus == PLANT_NEEDS_WATER) {
      plantsToWaterQueue.push(plant);
    }
  }
}
