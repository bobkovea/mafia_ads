#pragma once

#include "notes.h"

namespace Harry {

static constexpr uint16_t BaseTime = 1000U;

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


  // N(B4, Q), N(E5, DQ), N(G5, E),

  // N(FS5, Q), N(E5, H), N(B5, Q), N(A5, DH),  N(FS5, DH)

  N(B4, E),
  N(E5, DE), N(G5, S), N(FS5, E),
  N(E5, Q), N(B5, E),
  N(A5, DQ),  N(FS5, DQ)
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
