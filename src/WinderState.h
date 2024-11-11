#ifndef WINDERSTATE_H
#define WINDERSTATE_H

#include <Arduino.h>
#include "PinDef.h"
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include <OneButton.h>
#include <AccelStepper.h>
#define DISP_W 128
#define DISP_H 64
#define DISP_RST -1
#define DISP_ADDRESS 0x3D
// the state of the machine at any time
struct WinderState{
    uint32_t totalTurns = 1000;
    uint32_t currentTurns = 0;

    // running toggle
    bool isRunning = false;
    // speed in RPM of the final output shaft
    float coilRPM = 35.0f; 
};

// some limits for things
#define COIL_SPEED_MIN 40.0f
#define COIL_SPEED_MAX 

//===========================
class Winder{
private:
    WinderState state;
// outputs
    AccelStepper motor;
    Adafruit_SSD1306 disp;
// controls
    long encPos = 0;
    Encoder enc;
    OneButton encButton;
    OneButton startButton;
public:
    Winder();
    // call in setup
    void init();
    // call in loop
    void run();
};


#endif