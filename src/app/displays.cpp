#include "displays.h"

#include <Arduino.h>

// --- Переходы ---
ShiftTransition shiftTransition(lcd);
BacklightTransition backlightTransition(backlight);
DummyTransition dummyTransition;

// --- IdleScreens ---
TypingScreen idleScreen0(lcd, FPSTR(IdleText0), IdleScreen0TypingPeriodMs);
TypingScreen idleScreen1(lcd, FPSTR(IdleText1), IdleScreen1TypingPeriodMs);

// --- ArrowScreen ---
ArrowScreen arrowScreen(
    lcd,
    FPSTR(ArrowText0),
    FPSTR(ArrowText1),
    ArrowScreenTypingPeriodMs,
    ArrowsBlinkPeriodMs,
    ArrowsTextChangePeriodMs);

// --- EndingScreens ---
TypingScreen endingScreen0(lcd, FPSTR(EndingText0), EndingScreenTypingPeriodMs);
TypingScreen endingScreen1(lcd, FPSTR(EndingText1), EndingScreenTypingPeriodMs);
TypingScreen endingScreen2(lcd, FPSTR(EndingText2), EndingScreenTypingPeriodMs);

// --- BatteryScreen ---
BatteryScreen batteryScreen(lcd, batteryManager);

// --- TimeleftScreen ---
TimeleftScreen timeleftScreen(lcd, FPSTR(TimeleftText), TimeleftScreenTypingPeriodMs);

// --- StorageScreen ---
TypingScreen storageScreen(lcd, FPSTR(StorageText), StorageScreenTypingPeriodMs);

// --- IdleScreenset ---
static ScreenItem idleScreenItems[] = {
    {&idleScreen0, IdleScreen0DurationMs},
    {&idleScreen1, IdleScreen1DurationMs},
    {&arrowScreen, ArrowScreenDurationMs},
};
static constexpr uint8_t IdleScreenCount = sizeof(idleScreenItems) / sizeof(idleScreenItems[0]);
Screenset idleScreenset = {idleScreenItems, IdleScreenCount, IdleScreensetDurationMs, &backlightTransition};

// --- EndingScreenset ---
static ScreenItem endingScreenItems[] = {
    {&endingScreen0, EndingScreen0DurationMs},
    {&endingScreen1, EndingScreen1DurationMs},
    {&endingScreen2, EndingScreen2DurationMs},
};
static constexpr uint8_t EndingScreenCount = sizeof(endingScreenItems) / sizeof(endingScreenItems[0]);
Screenset endingScreenset = {
    endingScreenItems, EndingScreenCount, EndingScreensetDurationMs, &shiftTransition};

// --- BatteryScreenset ---
static ScreenItem batteryScreenItems[] = {{&batteryScreen, BatteryScreenDurationMs}};
static constexpr uint8_t BatteryScreenCount = sizeof(batteryScreenItems) / sizeof(batteryScreenItems[0]);
Screenset batteryScreenset = {
    batteryScreenItems, BatteryScreenCount, BatteryScreensetDurationMs, &dummyTransition};

// --- TimeleftScreenset ---
static ScreenItem timeleftScreenItems[] = {{&timeleftScreen, TimeleftScreenDurationMs}};
static constexpr uint8_t TimeleftScreenCount = sizeof(timeleftScreenItems) / sizeof(timeleftScreenItems[0]);
Screenset timeleftScreenset = {
    timeleftScreenItems, TimeleftScreenCount, TimeleftScreensetDurationMs, &dummyTransition};

// --- StorageScreenset ---
static ScreenItem storageScreenItems[] = {{&storageScreen, StorageScreenDurationMs}};
static constexpr uint8_t StorageScreenCount = sizeof(storageScreenItems) / sizeof(storageScreenItems[0]);
Screenset storageScreenset = {
    storageScreenItems, StorageScreenCount, StorageScreensetDurationMs, &dummyTransition};

// --- ScreenHandler ---
ScreenHandler screenHandler(&idleScreenset);