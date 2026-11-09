#pragma once

#include "../states/statemachine.h"

static constexpr uint8_t CardReaderDataPin = 2;
static constexpr uint8_t BuzzerPin = 5;
static constexpr uint8_t CardReaderPowerPin = 4;
static constexpr uint8_t BacklightPin = 9;
static constexpr uint8_t VbatPin = A0;
static constexpr uint16_t VccMv = (uint16_t)5100U; // вольметром
static constexpr uint8_t LcdI2cAddress = 0x27;
static constexpr uint8_t LcdRowsCount = 2;
static constexpr uint8_t LcdColsCount = 16;

// ============ Прототипы действий ============
void StartGoodCardIndication();
void StartBadCardIndication();
void StartSlotMachine();
void StartEnding();
void StartIdle();
void StartBatteryIndication();

// ============ Таблица переходов ============
const Transition transitions[] =
{
  { State::Idle, (uint8_t)Event::GoodCardDetected, State::GoodCardIndication, StartGoodCardIndication },
  { State::Idle, (uint8_t)Event::BadCardDetected, State::BadCardIndication, StartBadCardIndication },
  { State::Idle, (uint8_t)Event::BatteryIndicationRequested, State::BatteryIndication, StartBatteryIndication },
  { State::BatteryIndication, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle },
//  { State::BatteryIndication, (uint8_t)Event::GoodCardIndication, State::SlotMachine, StartSlotMachine },
  // { State::GoodCardIndication, (uint8_t)Event::IndicationFinished, State::SlotMachine, StartSlotMachine },
  { State::GoodCardIndication, (uint8_t)Event::IndicationFinished, State::Ending, StartEnding }, //todo test
  { State::BadCardIndication, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle },
  { State::SlotMachine, (uint8_t)Event::SlotMachineFinished, State::Ending, StartEnding },
  { State::Ending, (uint8_t)Event::IndicationFinished, State::Idle, StartIdle }
};

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");
