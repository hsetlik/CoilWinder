#include "WinderState.h"
#include <Wire.h>

Winder::Winder() : motor(AccelStepper::FULL2WIRE, STEPPER_STEP, STEPPER_DIR),
                   disp(DISP_W, DISP_H, &Wire, DISP_RST)
{
}

void Winder::encoderTurned(bool dir)
{
    constexpr uint32_t minTurns = 10;
    constexpr uint32_t maxTurns = 100000;
    constexpr uint32_t turnDiff = 10;

    if(!state.isRunning){
        if(dir){
            state.totalTurns = std::min<uint32_t>(state.totalTurns + turnDiff, maxTurns);
        } else {

            state.totalTurns = std::max<uint32_t>(state.totalTurns - turnDiff, minTurns);
        }
    }


}
void Winder::startButtonPressed()
{
    state.isRunning = !state.isRunning;
    // stop the motor quickly if
    if(!state.isRunning){
        motor.stop();
        motor.runToPosition();
    } else {
        speedPotChanged(speedPotVal);
    }
}
void Winder::encButtonPressed()
{
    if(!state.isRunning){ // reset the turn counter
        currentCoilTurns = 0.0f;
    }
}
void Winder::speedPotChanged(uint16_t val){
    speedPotVal = val;
    float fNorm = (float)val / 4095.0f;
    state.coilRPM = COIL_SPEED_MIN + ((COIL_SPEED_MAX - COIL_SPEED_MIN) * fNorm);
    float revsPerSecond = state.coilRPM / 240.0f;
    motor.setSpeed(revsPerSecond * (float)STEPS_PER_MOTOR_REV);
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
    motor.setMaxSpeed(1000.0f);
    motor.setAcceleration(100.0f);
}

void Winder::run()
{
    if(state.isRunning){
        motor.runSpeed();
        incrementTurns(motor.currentPosition());
    }
}


void Winder::incrementTurns(int32_t pos){
    static int32_t prevPos = 0;
    // turns = dSteps / steps per rev
    float diff = (float)(pos - prevPos)/(float)(STEPS_PER_MOTOR_REV / 4);
    currentCoilTurns += diff;
    if((uint32_t)currentCoilTurns >= state.totalTurns){ // we've reached the end of the coil
        motor.stop();
        motor.runToPosition();
        state.isRunning = false;
    }
}