#pragma once

#include <Arduino.h>

// --- Typing periods ---
static constexpr uint32_t IdleScreen0TypingPeriodMs = 120U;
static constexpr uint32_t IdleScreen1TypingPeriodMs = 120U;
static constexpr uint32_t ArrowScreenTypingPeriodMs = 60U;
static constexpr uint32_t EndingScreenTypingPeriodMs = 50U;
static constexpr uint32_t BatteryScreenTypingPeriodMs = 120U;
static constexpr uint32_t TimeleftScreenTypingPeriodMs = 60U;

// --- Blink / change ---
static constexpr uint32_t ArrowsBlinkPeriodMs = 400U;
static constexpr uint32_t ArrowsTextChangePeriodMs = 4000U;

// --- Screen durations ---
static constexpr uint32_t IdleScreen0DurationMs = 7000U;
static constexpr uint32_t IdleScreen1DurationMs = 7000U;
static constexpr uint32_t ArrowScreenDurationMs = 15000U;
static constexpr uint32_t EndingScreen0DurationMs = 4000U;
static constexpr uint32_t EndingScreen1DurationMs = 8000U;
static constexpr uint32_t EndingScreen2DurationMs = 5000U;
static constexpr uint32_t BatteryScreenDurationMs = 20000U;
static constexpr uint32_t TimeleftScreenDurationMs = UINT32_MAX;

// --- Screenset durations ---
static constexpr uint32_t IdleScreensetDurationMs = UINT32_MAX;
static constexpr uint32_t EndingScreensetDurationMs = 20000U;
static constexpr uint32_t BatteryScreensetDurationMs = 5000U;
static constexpr uint32_t TimeleftScreensetDurationMs = 5000U;