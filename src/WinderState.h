#ifndef WINDERSTATE_H
#define WINDERSTATE_H

#include <Arduino.h>
#include "PinDef.h"
#include <Adafruit_SSD1306.h>

#include <AccelStepper.h>
#define DISP_W 128
#define DISP_H 64
#define DISP_RST -1
#define DISP_ADDRESS 0x3D

#define STEPS_PER_MOTOR_REV 200
// the state of the machine at any time
struct WinderState{
    uint32_t totalTurns = 1000;

    // running toggle
    bool isRunning = false;
    // speed in RPM of the final output shaft
    float coilRPM = 35.0f;
};

// some limits for things
#define COIL_SPEED_MIN 20.0f
#define COIL_SPEED_MAX 1500.0f

//===========================
class Winder{
private:
    WinderState state;
    float currentCoilTurns = 0.0f;
// outputs
    AccelStepper motor;
    Adafruit_SSD1306 disp;
// helper for counting the turns
    uint16_t speedPotVal = 0;
    void incrementTurns(int32_t pos);
public:
    Winder();
    // call in setup
    void init();
    // call in loop
    void run();
    // wire these to the button/encoder callbacks in main.cpp
    void encoderTurned(bool dir);
    void startButtonPressed();
    void encButtonPressed();
    void speedPotChanged(uint16_t val);
};


#endif