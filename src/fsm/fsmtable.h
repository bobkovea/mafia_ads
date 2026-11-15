#pragma once
#include <Arduino.h>
#include "states.h"
#include "statehandlers.h"

void StartIdle();
void StartBatteryIndication();
void StartGoodCardIndication();
void StartBadCardIndication();
void StartSlotMachine();
void StartEnding();

// ============ Таблица переходов ============
static constexpr Transition transitions[] = {
    {State::Idle, (uint8_t)Event::GoodCardDetected, State::GoodCardIndication, StartGoodCardIndication},
    {State::Idle, (uint8_t)Event::BadCardDetected, State::BadCardIndication, StartBadCardIndication},
    {State::Idle,
     (uint8_t)Event::BatteryIndicationRequested,
     State::BatteryIndication,
     StartBatteryIndication},
    {State::BatteryIndication, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle},
    //  { State::BatteryIndication, (uint8_t)Event::GoodCardIndication, State::SlotMachine, StartSlotMachine
    //  },
    // { State::GoodCardIndication, (uint8_t)Event::IndicationFinished, State::SlotMachine, StartSlotMachine
    // },
    {State::GoodCardIndication, (uint8_t)Event::IndicationFinished, State::Ending, StartEnding}, // todo test
    {State::BadCardIndication, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle},
    {State::SlotMachine, (uint8_t)Event::SlotMachineFinished, State::Ending, StartEnding},
    {State::Ending, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle}};

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");
