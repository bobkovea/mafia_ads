#pragma once

#include "melodybase.h"

namespace Mario
{
	
static constexpr uint16_t Tempo = 933U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{
  // Такт 1
  N(E5, S), N(E5, E), N(E5, E), N(C5, S), N(E5, E), N(G5, H),

  // Такт 2
  N(C5, DE), N(G4, DE), N(E4, DE), N(A4, E), N(B4, E), N(AS4, S), N(A4, E),

  // Такт 3
  N(G4, S), N(E5, E), N(G5, S), N(A5, E), N(F5, S), N(G5, E), N(E5, E),
  N(C5, S), N(D5, S), N(B4, DE),

  // Такт 4
  N(C5, DE), N(G4, DE), N(E4, DE), N(A4, E), N(B4, E), N(AS4, S), N(A4, E),

  // Такт 5
  N(G4, S), N(E5, E), N(G5, S), N(A5, E), N(F5, S), N(G5, E), N(E5, E),
  N(C5, S), N(D5, S), N(B4, DE),

  // Такт 6
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(GS4, S), N(A4, S), N(C5, E), N(A4, S), N(C5, S), N(D5, S),

  // Такт 7
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(C6, E), N(C6, S), N(C6, Q),

  // Такт 8
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(GS4, S), N(A4, S), N(C5, E), N(A4, S), N(C5, S), N(D5, S),

  // Такт 9
  N(REST, E), N(DS5, DE), N(D5, DE), N(C5, H),

  // Такт 10
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(GS4, S), N(A4, S), N(C5, E), N(A4, S), N(C5, S), N(D5, S),

  // Такт 11
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(C6, E), N(C6, S), N(C6, Q),

  // Такт 12
  N(REST, E), N(G5, S), N(FS5, S), N(F5, S), N(DS5, E), N(E5, E),
  N(GS4, S), N(A4, S), N(C5, E), N(A4, S), N(C5, S), N(D5, S),

  // Такт 13
  N(REST, E), N(DS5, DE), N(D5, DE), N(C5, H),

  // Такт 14
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, E), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 15
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, S), N(E5, S), N(REST, H),

  // Такт 16
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, E), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 17
  N(E5, S), N(E5, E), N(E5, E), N(C5, S), N(E5, E), N(G5, H),

  // Такт 18
  N(C5, DE), N(G4, DE), N(E4, DE), N(A4, E), N(B4, E), N(AS4, S), N(A4, E),

  // Такт 19
  N(G4, S), N(E5, E), N(G5, S), N(A5, E), N(F5, S), N(G5, E), N(E5, E),
  N(C5, S), N(D5, S), N(B4, DE),

  // Такт 20
  N(C5, DE), N(G4, DE), N(E4, DE), N(A4, E), N(B4, E), N(AS4, S), N(A4, E),

  // Такт 21
  N(G4, S), N(E5, E), N(G5, S), N(A5, E), N(F5, S), N(G5, E), N(E5, E),
  N(C5, S), N(D5, S), N(B4, DE),

  // Такт 22
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 23
  N(B4, S), N(A5, E), N(A5, S), N(A5, S), N(G5, E), N(F5, S), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 24
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 25
  N(B4, S), N(F5, E), N(F5, S), N(F5, S), N(E5, E), N(D5, S), N(C5, H),

  // Такт 26
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 27
  N(B4, S), N(A5, E), N(A5, S), N(A5, S), N(G5, E), N(F5, S), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 28
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 29
  N(B4, S), N(F5, E), N(F5, S), N(F5, S), N(E5, E), N(D5, S), N(C5, H),

  // Такт 30
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, E), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 31
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, S), N(E5, S), N(REST, H),

  // Такт 32
  N(C5, S), N(C5, E), N(C5, E), N(C5, S), N(D5, E), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 33
  N(E5, S), N(E5, E), N(E5, E), N(C5, S), N(E5, E), N(G5, H),

  // Такт 34
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 35
  N(B4, S), N(A5, E), N(A5, S), N(A5, S), N(G5, E), N(F5, S), N(E5, S),
  N(C5, E), N(A4, S), N(G4, Q),

  // Такт 36
  N(E5, S), N(C5, E), N(G4, DE), N(GS4, E), N(A4, S),
  N(F5, E), N(F5, S), N(A4, Q),

  // Такт 37
  N(B4, S), N(F5, E), N(F5, S), N(F5, S), N(E5, E), N(D5, S), N(C5, H),
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
