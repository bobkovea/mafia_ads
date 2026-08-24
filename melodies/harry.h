#pragma once

#include "melodybase.h"

namespace Harry
{

static constexpr uint16_t Tempo = 1000U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  N(B4, E),
  N(E5, DE), N(G5, S), N(FS5, E),
  N(E5, Q), N(B5, E),
  N(A5, DQ), N(FS5, DQ)
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
