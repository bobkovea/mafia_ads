#pragma once

#include "melodybase.h"

namespace DoorBeep
{
	
static constexpr uint16_t Tempo = 1050U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  N(FS7, DE), N(REST, Q), N(FS7, S), N(REST, S),  N(FS7, S)
};
	
const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}


