#pragma once

#include <Arduino.h>

// ============ Состояния ============
enum class State : uint8_t
{
    Idle,
    BatteryIndication,
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
    TimeleftCardDetected = 0x02,
    StorageIsFull = 0x04,
    IndicationFinished = 0x08,
    SlotMachineFinished = 0x10,
    BatteryIndicationRequested = 0x20
};

// ============ Прототип действия ============
typedef void (*ActionCallback)();

// ============ Структура перехода ============z
struct Transition
{
    State currentState;
    Event event;
    State nextState;
    ActionCallback action;
};