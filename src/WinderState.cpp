#include "WinderState.h"
#include <Wire.h>

Winder::Winder() : 
motor(AccelStepper::FULL2WIRE, STEPPER_STEP, STEPPER_DIR), 
disp(DISP_W, DISP_H, &Wire, DISP_RST), 
enc(ENC_L, ENC_R), 
encButton(ENC_BTN, true), 
startButton(START_BTN, true)
{
}

void Winder::init()
{
    Serial.begin(115200);

    // init the display
    if (!disp.begin(SSD1306_SWITCHCAPVCC, DISP_ADDRESS))
    {
        Serial.println("Could not initialize display!");
    }

    // init the motor
    motor.setMaxSpeed(300.0f);
    motor.setAcceleration(100.0f);

}

void Winder::run()
{
}