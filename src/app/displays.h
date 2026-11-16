#pragma once

#include <Arduino.h>

#include "../config/texts.h"
#include "../config/timings.h"

#include "../ui/screens/typingscreen.h"
#include "../ui/screens/arrowscreen.h"
#include "../ui/screens/batteryscreen.h"
#include "../ui/screens/timeleftscreen.h"
#include "../ui/screenhandler.h"
#include "../ui/transitions/backlighttransition.h"
#include "../ui/transitions/shifttransition.h"
#include "../ui/transitions/dummytransition.h"

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
extern TypingScreen storageScreen;

// --- Наборы экранов ---
extern Screenset idleScreenset;
extern Screenset endingScreenset;
extern Screenset batteryScreenset;
extern Screenset timeleftScreenset;
extern Screenset storageScreenset;

// --- Обработчик ---
extern ScreenHandler screenHandler;