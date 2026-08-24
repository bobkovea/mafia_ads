#pragma once

#include "melodybase.h"

static constexpr uint16_t Tempo = 1333U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

namespace Godfather
{
	
const Note melody[] PROGMEM =
{
  N(REST, Q), N(B3, Q), N(E4, Q), N(G4, Q),
  N(FS4, Q), N(E4, Q), N(G4, Q), N(E4, Q), N(FS4, Q), N(E4, Q), N(C4, Q), N(D4, Q),
  N(B3, W),
  
  N(REST, Q), N(B3, Q), N(E4, Q), N(G4, Q),
  N(FS4, Q), N(E4, Q), N(G4, Q), N(E4, Q), N(FS4, Q), N(E4, Q), N(B3, Q), N(AS3, Q),
  N(A3, W),
  
  N(REST, Q), N(A3, Q), N(C4, Q), N(DS4, Q),
  N(FS4, W),
  N(REST, Q), N(A3, Q), N(C4, Q), N(DS4, Q),
  N(E4, W),
  
  N(REST, Q), N(E3, Q), N(G3, Q), N(D4, Q),
  N(C4, Q), N(B3, Q), N(D4, Q), N(C4, Q),
  N(C4, Q), N(B3, Q), N(B3, Q), N(DS4, Q),
  N(E4, W)
};
	
const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}


