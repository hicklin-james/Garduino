#include "Greenhouse.h"

struct PlantAtts Greenhouse::plant1 = {"Basil",2,0,3,4,375,405};
struct PlantAtts Greenhouse::plant2 = {"Sage",5,1,6,-1,375,405};
struct PlantAtts Greenhouse::initialPlantAtts[NUM_PLANTS] = {plant1, plant2};

const char ssid[8] = "MuadDib";
const char password[14] = "Lexington1775";

/**
  Constructor
  Description:
    Initializes plant arrays using static variables defined in header file.
**/
Greenhouse::Greenhouse() : _reqManager(ssid, password, 10, 11) {

  for (int i = 0; i < NUM_PLANTS; i++) {
    struct PlantAtts plantAtts = initialPlantAtts[i];
    Plant *plant = new Plant(plantAtts.name, plantAtts.lowerMoistureThreshold, plantAtts.upperMoistureThreshold, plantAtts.moisturePin, plantAtts.moisturePowerPin, plantAtts.growLightPin, plantAtts.solenoidPin);
    plants[i] = plant;
  }
  currentlyWatering = NULL;

  pinMode(WATERING_PIN_LED, OUTPUT);
}

/**
  Destructor
  Description:
    Deallocs plants
**/
Greenhouse::~Greenhouse() {
  for (int i = 0; i < NUM_PLANTS; i++) {
    Plant *plant = plants[i];
    delete plant;
  }
}

/**
  Returns: void
  Description:
    Main workhorse of the greenhouse. It is responsible for managing the watering queue
    and starting the watering process of the plants.
**/
void Greenhouse::poll() {
  // if there are any plants in the queue waiting to be watered, and no plant is currently
  // being watered, pop and water.
  if (!plantsToWaterQueue.isEmpty() && (currentlyWatering == NULL || !currentlyWatering->isWatering())) {
    currentlyWatering = plantsToWaterQueue.pop();
    currentlyWatering->startWatering();
    digitalWrite(WATERING_PIN_LED, HIGH);
  }
  else if (!currentlyWatering || !currentlyWatering->isWatering()) {
    digitalWrite(WATERING_PIN_LED, LOW);
  }

  pollPlants(); 
}

/**
  Returns: void
  Description:
    Polls each plant and adds it to the watering queue if it needs to be watered.
**/
void Greenhouse::pollPlants() {
  for (int i = 0; i < NUM_PLANTS; i++) {
    Plant *plant = plants[i];
    plant->evaluateGrowLights();
    int plantStatus = plant->pollPlantSensor(_reqManager);
    // If the plant indicates that it needs to be watered, add it to the plantsToWater queue
    if (plantStatus == PLANT_NEEDS_WATER) {
      plantsToWaterQueue.push(plant);
    }
  }
}
