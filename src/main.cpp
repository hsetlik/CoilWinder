#include <Arduino.h>
#include <Encoder.h>
#include <OneButton.h>

#include "WinderState.h"

// controls
long encPos = 0;
Encoder enc(ENC_L, ENC_R);
OneButton encButton;
OneButton startButton;

uint16_t speedPotVal = 0;
uint16_t potReading;

// main business stuff
Winder w;

// callback pointers
void encButtonClicked(){
  w.encButtonPressed();
}

void startButtonClicked(){
  w.startButtonPressed();
}

void setup()
{
  // set up buttons
  encButton.setup(ENC_BTN, INPUT, true);
  startButton.setup(START_BTN, INPUT, true);
  encButton.attachClick(encButtonClicked);
  startButton.attachClick(startButtonClicked);

  w.init();
}

void loop()
{
  // check the buttons
  encButton.tick();
  startButton.tick();
  // check the encoder
  long newPos = enc.read();
  if(newPos != encPos){
    w.encoderTurned(newPos > encPos);
    encPos = newPos;
  }
  // check the speed knob
  potReading = analogRead(SPEED_POT_PIN);
  if(potReading != speedPotVal){
    w.speedPotChanged(potReading);
    speedPotVal = potReading;
  }
  // update the motor and display
  w.run();
}
