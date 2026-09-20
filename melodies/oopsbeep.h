#pragma once

#include "melodybase.h"

namespace OopsBeep
{
	
static constexpr uint16_t Tempo = 1050U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  N(FS4, E), N(FS4, E)
};
	
const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}


