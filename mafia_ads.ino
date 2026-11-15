#include "src/app/peripherals.h"
#include "src/app/displays.h"
#include "src/app/statemachine_global.h"
#include "src/config/pins.h"
#include "src/fsm/statehandlers.h"

void setup()
{
    randomSeed(analogRead(A0));
    pinMode(BuzzerPin, OUTPUT);
    pinMode(CardReaderPowerPin, OUTPUT);
    digitalWrite(CardReaderPowerPin, HIGH);
    pinMode(BacklightPin, OUTPUT);
    digitalWrite(BacklightPin, HIGH);
    lcd.begin();
    screenHandler.Begin();
    rfid.begin(9600);
}

void loop()
{
    const State currentState = stateMachine.Process();
    const uint32_t now = millis();
    HandleState(currentState, now);
}