
#pragma once

#include "melodybase.h"

namespace Aha
{

static constexpr uint16_t Tempo = 1400U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  N(FS5, E), N(FS5, E), N(D5, E), N(B4, E), N(REST, E), N(B4, E), N(REST, E), N(E5, E),
  N(REST, E), N(E5, E), N(REST, E), N(E5, E), N(GS5, E), N(GS5, E), N(A5, E), N(B5, E),
  N(A5, E), N(A5, E), N(A5, E), N(E5, E), N(REST, E), N(D5, E), N(REST, E), N(FS5, E),
  N(REST, E), N(FS5, E), N(REST, E), N(FS5, E), N(E5, E), N(E5, E), N(FS5, E), N(E5, E),
  N(FS5, E), N(FS5, E), N(D5, E), N(B4, E), N(REST, E), N(B4, E), N(REST, E), N(E5, E),

  N(REST, E), N(E5, E), N(REST, E), N(E5, E), N(GS5, E), N(GS5, E), N(A5, E), N(B5, E),
  N(A5, E), N(A5, E), N(A5, E), N(E5, E), N(REST, E), N(D5, E), N(REST, E), N(FS5, E),
  N(REST, E), N(FS5, E), N(REST, E), N(FS5, E), N(E5, E), N(E5, E), N(FS5, E), N(E5, E),
  N(FS5, E), N(FS5, E), N(D5, E), N(B4, E), N(REST, E), N(B4, E), N(REST, E), N(E5, E),
  N(REST, E), N(E5, E), N(REST, E), N(E5, E), N(GS5, E), N(GS5, E), N(A5, E), N(B5, E),

  N(A5, E), N(A5, E), N(A5, E), N(E5, E), N(REST, E), N(D5, E), N(REST, E), N(FS5, E),
  N(REST, E), N(FS5, E), N(REST, E), N(FS5, E), N(E5, E), N(E5, E), N(FS5, E), N(E5, E)
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
