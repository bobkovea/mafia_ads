#pragma once

#include "melodybase.h"

namespace PinkPanther
{
	
static constexpr uint16_t Tempo = 2000U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
	/*
  N(REST, H), N(REST, Q), N(DS4, E),
  N(E4, DQ), N(REST, E), N(FS4, E), N(G4, DQ), N(REST, E), N(DS4, E),
  N(E4, DE), N(FS4, E), N(G4, DE), N(C5, E), N(B4, DE), N(E4, E), N(G4, DE), N(B4, E),
  N(AS4, H), N(A4, DS), N(G4, DS), N(E4, DS), N(D4, DS),
  N(E4, H), N(REST, Q), N(REST, E), N(DS4, Q),

  N(E4, DQ), N(REST, E), N(FS4, E), N(G4, DQ), N(REST, E), N(DS4, E),
  N(E4, DE), N(FS4, E),  N(G4, DE), N(C5, E), N(B4, DE), N(G4, E), N(B4, DE), N(E5, E),
  N(DS5, W),
  
  N(D5, H), N(REST, Q), N(REST, E), */N(DS4, E),
  
  N(E4, DQ), N(REST, E), N(FS4, E), N(G4, DQ), N(REST, E), N(DS4, E),
  N(E4, DE), N(FS4, E), N(G4, DE), N(C5, E), N(B4, DE), N(E4, E), N(G4, DE), N(B4, E),

  N(AS4, H), N(A4, DS), N(G4, DS), N(E4, DS), N(D4, DS),
  N(E4, DQ), N(REST, Q),
  N(REST, Q), N(E5, DE), N(D5, E), N(B4, DE), N(A4, E), N(G4, DE), N(E4, DE),
  N(AS4, S), N(A4, DE), N(AS4, S), N(A4, DE), N(AS4, S), N(A4, DE), N(AS4, S), N(A4, DE),
  N(G4, DS), N(E4, DS), N(D4, DS), N(E4, S), N(E4, S), N(E4, H)
};
	
const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}


