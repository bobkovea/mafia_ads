#pragma once
#include <Arduino.h>
#include "states.h"

void StartIdle();
void StartBatteryIndication();
void StartGoodCardIndication();
void StartTimeleftCardIndication();
void StartStorageIsFullIndication();
void StartSlotMachine();
void StartEnding();

void HandleState(const State state, const uint32_t now);