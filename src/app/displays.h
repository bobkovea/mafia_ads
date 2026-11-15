#pragma once

#include <Arduino.h>

#include "../config/texts.h"
#include "../config/timings.h"

#include "../display/screens/typingscreen.h"
#include "../display/screens/arrowscreen.h"
#include "../display/screens/batteryscreen.h"
#include "../display/screens/timeleftscreen.h"
#include "../display/screenhandler.h"
#include "../display/transitions/backlighttransition.h"
#include "../display/transitions/shifttransition.h"
#include "../display/transitions/dummytransition.h"

#include "peripherals.h"

// --- Переходы ---
extern ShiftTransition shiftTransition;
extern BacklightTransition backlightTransition;
extern DummyTransition dummyTransition;

// --- Экраны ---
extern TypingScreen idleScreen0;
extern TypingScreen idleScreen1;
extern ArrowScreen arrowScreen;
extern TypingScreen endingScreen0;
extern TypingScreen endingScreen1;
extern TypingScreen endingScreen2;
extern BatteryScreen batteryScreen;
extern TimeleftScreen timeleftScreen;

// --- Наборы экранов ---
extern Screenset idleScreenset;
extern Screenset endingScreenset;
extern Screenset batteryScreenset;
extern Screenset timeleftScreenset;

// --- Обработчик ---
extern ScreenHandler screenHandler;