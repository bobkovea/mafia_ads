#pragma once

#include <Arduino.h> 

static constexpr uint8_t CardReaderDataPin = 2;
static constexpr uint8_t BuzzerPin = 5;
static constexpr uint8_t CardReaderPowerPin = 4;
static constexpr uint8_t BacklightPin = 9;
static constexpr uint8_t VbatPin = A0;
static constexpr uint16_t VccMv = (uint16_t)5100U; // вольметром
static constexpr uint8_t LcdI2cAddress = 0x27;
static constexpr uint8_t LcdRowsCount = 2;
static constexpr uint8_t LcdColsCount = 16;