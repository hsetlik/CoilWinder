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
        displayUpdateNeeded = true;
    }


}
void Winder::startButtonPressed()
{
    state.isRunning = !state.isRunning;
    // stop the motor quickly if
    if(!state.isRunning){
        motor.stop();
        motor.runToPosition();
    } else if (currentCoilTurns < (float)state.totalTurns) {
        speedPotChanged(speedPotVal);
    }
}
void Winder::encButtonPressed()
{
    if(!state.isRunning){ // reset the turn counter
        currentCoilTurns = 0.0f;
        displayUpdateNeeded = true;
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
    // init I2C
    if(!Wire.begin(SDA_PIN, SCL_PIN)){
        Serial.println("Could not start I2C!");
    }

    // init the display
    if (!disp.begin(SSD1306_SWITCHCAPVCC, DISP_ADDRESS))
    {
        Serial.println("Could not initialize display!");
    }

    // init the motor
    motor.setMaxSpeed(2000.0f);
    motor.setAcceleration(100.0f);
}

void Winder::run()
{
    if(state.isRunning){
        motor.runSpeed();
        incrementTurns(motor.currentPosition());
    }
    if(displayUpdateNeeded){
        updateDisplay();
        displayUpdateNeeded = false;
    }
}


void Winder::incrementTurns(int32_t pos){
    static int32_t prevPos = 0;
    // turns = dSteps / steps per rev
    float diff = (float)(pos - prevPos)/(float)(STEPS_PER_MOTOR_REV / 4);
    prevPos = pos;
    currentCoilTurns += diff;
    if((uint32_t)currentCoilTurns >= state.totalTurns){ // we've reached the end of the coil
        motor.stop();
        motor.runToPosition();
        state.isRunning = false;
    }
    if(currentCoilTurns - lastDrawnCoilTurns > 1.0f){
        displayUpdateNeeded = true;
        lastDrawnCoilTurns = currentCoilTurns;
    }
}

void Winder::updateDisplay(){
    disp.clearDisplay();
    disp.setTextSize(1);
    disp.setTextColor(SSD1306_WHITE);
    disp.setCursor(4, 0);
    disp.print("TURNS:");
    std::string numStr = std::to_string((uint16_t)currentCoilTurns) + "/" + std::to_string(state.totalTurns);
    disp.setCursor(4, 8);
    disp.print(numStr.c_str());
    disp.display();
}