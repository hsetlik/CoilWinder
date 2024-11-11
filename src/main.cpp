#include <Arduino.h>

#include "WinderState.h"

Winder w;
void setup(){
  w.init();
}

void loop() {
  w.run();
}

