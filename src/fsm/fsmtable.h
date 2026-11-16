#pragma once
#include <Arduino.h>
#include "states.h"
#include "statehandlers.h"

// clang-format off

// ============ Таблица переходов ============
static constexpr Transition transitions[] = {
    {State::Idle, Event::GoodCardDetected, State::GoodCardIndication, StartGoodCardIndication},
    {State::Idle, Event::TimeleftCardDetected, State::BadCardIndication, StartTimeleftCardIndication},
    {State::Idle, Event::StorageIsFull, State::BadCardIndication, StartStorageIsFullIndication},
    {State::Idle, Event::BatteryIndicationRequested, State::BatteryIndication, StartBatteryIndication},
    {State::BatteryIndication, Event::IndicationFinished, State::Idle, StartIdle},
    // { State::GoodCardIndication, Event::IndicationFinished, State::SlotMachine, StartSlotMachine
    // },
    {State::GoodCardIndication, Event::IndicationFinished, State::Ending, StartEnding}, // todo test
    {State::BadCardIndication, Event::IndicationFinished, State::Idle, StartIdle},
    {State::SlotMachine, Event::SlotMachineFinished, State::Ending, StartEnding},
    {State::Ending, Event::IndicationFinished, State::Idle, StartIdle}};

// clang-format on

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");
