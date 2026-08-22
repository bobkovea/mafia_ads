#pragma once

#include "notes.h"

namespace Godfather {

static constexpr uint16_t BaseTime = 1750U;

enum class Duration : uint16_t {
  W = BaseTime,
  H = W / 2,
  Q = W / 4,
  E = W / 8,
  S = W / 16,
  DE = E + S,
  DQ = Q + E,
  DH = H + Q
};

// Макрос для создания ноты
#define N(note, dur) { NOTE_##note, (uint16_t)Duration::dur }

const Note melody[] PROGMEM = {
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
