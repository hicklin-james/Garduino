#ifndef __GREENHOUSE_H_INCLUDED__
#define __GREENHOUSE_H_INCLUDED__

#define NUM_PLANTS 2
#define MAX_PLANTS 10

#include "Plant.h"
#include <Arduino.h>
#include <QueueList.h>

// Struct used to declare new plants. Add new PlantAtts as static vars below.
struct PlantAtts {
  String name;
  int solenoidPin;
  int moisturePin;
  int lowerMoistureThreshold;
  int upperMoistureThreshold;
};

class Greenhouse {
  public:
    Greenhouse();
    ~Greenhouse();
    void poll();
  private:
    Plant *plants[MAX_PLANTS]; // Array of plants, filled up in constructor
    QueueList<Plant*> plantsToWaterQueue; // Queue of plants that need to be watered
    Plant *currentlyWatering; // Plant that is currently being watered

    void pollPlants();

    // Constants used in constructor
    // Add new plants at will - make sure to change NUM_PLANTS as well
    static struct PlantAtts plant1; //= {"Basil",2,0,900,800};
    static struct PlantAtts plant2; //= {"Rosemary",2,0,900,800};
    static struct PlantAtts initialPlantAtts[NUM_PLANTS]; //= {plant1, plant2};
};


#endif