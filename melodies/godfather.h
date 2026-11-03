#pragma once

#include "melodybase.h"

namespace Godfather
{
	
static constexpr uint16_t Tempo = 1850U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  N(REST, Q), N(G4, Q), N(C5, Q), N(DS5, Q),
  N(D5, Q), N(C5, Q), N(DS5, Q), N(C5, Q), N(D5, Q), N(C5, Q), N(GS4, Q), N(AS4, Q),
  N(G4, W),
  
  N(REST, Q), N(G4, Q), N(C5, Q), N(DS5, Q),
  N(D5, Q), N(C5, Q), N(DS5, Q), N(C5, Q), N(D5, Q), N(C5, Q), N(G4, Q), N(FS4, Q),
  N(F4, W),
  
  N(REST, Q), N(F4, Q), N(GS4, Q), N(B4, Q),
  N(D5, W),
  N(REST, Q), N(F4, Q), N(GS4, Q), N(B4, Q),
  N(C5, W),
  
  N(REST, Q), N(C4, Q), N(DS4, Q), N(AS4, Q),
  N(GS4, Q), N(G4, Q), N(AS4, Q), N(GS4, Q),
  N(GS4, Q), N(G4, Q), N(G4, Q), N(B3, Q),
  N(C4, W)
};
	
const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}


