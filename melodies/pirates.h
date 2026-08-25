
#pragma once

#include "melodybase.h"

namespace Pirates
{

static constexpr uint16_t Tempo = 1050U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
/*
  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(C5, E), N(D5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(A4, E), N(G4, E), N(A4, DQ), N(REST, E),

  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(C5, E), N(D5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(A4, E), N(G4, E), N(A4, DQ), N(REST, E),

  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
  N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
  N(E5, E), N(D5, E), N(E5, E), N(A4, Q), N(REST, E),

  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(D5, Q), N(E5, E), N(A4, Q), N(REST, E),
  N(A4, E), N(C5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(C5, E), N(A4, E), N(B4, DQ), N(REST, DQ),

  N(A4, Q), N(A4, E),*/
  //Repeat of first part
  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(C5, E), N(D5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(A4, E), N(G4, E), N(A4, DQ), N(REST, E),

  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(C5, E), N(D5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(A4, E), N(G4, E), N(A4, DQ), N(REST, E),

  N(E4, E), N(G4, E), N(A4, Q), N(A4, E), N(REST, E),
  N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
  N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
  N(E5, E), N(D5, E), N(E5, E), N(A4, Q), N(REST, E),

  N(A4, E), N(B4, E), N(C5, Q), N(C5, E), N(REST, E),
  N(D5, Q), N(E5, E), N(A4, Q), N(REST, E),
  N(A4, E), N(C5, E), N(B4, Q), N(B4, E), N(REST, E),
  N(C5, E), N(A4, E), N(B4, DQ), N(REST, DQ),
  //End of Repeat

  N(E5, Q), N(REST, E), N(REST, DQ), N(F5, Q), N(REST, E), N(REST, DQ),
  N(E5, E), N(E5, E), N(REST, E), N(G5, E), N(REST, E), N(E5, E), N(D5, E), N(REST, E), N(REST, DQ),
  N(D5, Q), N(REST, E), N(REST, DQ), N(C5, Q), N(REST, E), N(REST, DQ),
  N(B4, E), N(C5, E), N(REST, E), N(B4, E), N(REST, E), N(A4, H),

  N(E5, Q), N(REST, E), N(REST, DQ), N(F5, Q), N(REST, E), N(REST, DQ),
  N(E5, E), N(E5, E), N(REST, E), N(G5, E), N(REST, E), N(E5, E), N(D5, E), N(REST, E), N(REST, DQ),
  N(D5, Q), N(REST, E), N(REST, DQ), N(C5, Q), N(REST, E), N(REST, DQ),
  N(B4, E), N(C5, E), N(REST, E), N(B4, E), N(REST, E), N(A4, H),
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
