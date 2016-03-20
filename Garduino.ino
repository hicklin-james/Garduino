// --------------------------------------------------------------------------------------- //
// for some stupid reason, arduino needs you to include the library header in the main ino
// file before you can include it in linked h files. 
#include <Time.h>
#include <QueueList.h>
// --------------------------------------------------------------------------------------- //

// --------------------------------------------------------------------------------------- //
// Main includes
#include "Greenhouse.h"
// --------------------------------------------------------------------------------------- //

Greenhouse *greenHouse;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  greenHouse = new Greenhouse();
  millis();
}

void loop() {
  greenHouse->poll();
}
