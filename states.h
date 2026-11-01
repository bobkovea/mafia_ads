#pragma once

// ============ Состояния ============
enum class State : uint8_t
{
  Idle,
  GoodCardIndication,
  BadCardIndication,
  SlotMachine,
  Ending
};

// ============ События ============
enum class Event : uint8_t
{
  None = 0x00,
  GoodCardDetected = 0x01,
  BadCardDetected = 0x02,
  CardIndicationFinished = 0x04,
  SlotMachineFinished = 0x08,
  EndingFinished = 0x10
};

// ============ Прототип действия ============
typedef void (*ActionCallback)();

// ============ Структура перехода ============
struct Transition
{
  State currentState;
  uint8_t event;
  State nextState;
  ActionCallback action;
};
