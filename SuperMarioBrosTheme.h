#pragma once
#include "notes.h"

namespace Mario {

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
  // Такт 1
  N(E6, S), N(E6, E), N(E6, E), N(C6, S), N(E6, E), N(G6, H),

  // Такт 2
  N(C6, DE), N(G5, DE), N(E5, DE), N(A5, E), N(B5, E), N(AS5, S), N(A5, E),

  // Такт 3
  N(G5, S), N(E6, E), N(G6, S), N(A6, E), N(F6, S), N(G6, E), N(E6, E),
  N(C6, S), N(D6, S), N(B5, DE),

  // Такт 4
  N(C6, DE), N(G5, DE), N(E5, DE), N(A5, E), N(B5, E), N(AS5, S), N(A5, E),

  // Такт 5
  N(G5, S), N(E6, E), N(G6, S), N(A6, E), N(F6, S), N(G6, E), N(E6, E),
  N(C6, S), N(D6, S), N(B5, DE),

  // Такт 6
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(GS5, S), N(A5, S), N(C6, E), N(A5, S), N(C6, S), N(D6, S),

  // Такт 7
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(C7, E), N(C7, S), N(C7, Q),

  // Такт 8
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(GS5, S), N(A5, S), N(C6, E), N(A5, S), N(C6, S), N(D6, S),

  // Такт 9
  N(REST, E), N(DS6, DE), N(D6, DE), N(C6, H),

  // Такт 10
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(GS5, S), N(A5, S), N(C6, E), N(A5, S), N(C6, S), N(D6, S),

  // Такт 11
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(C7, E), N(C7, S), N(C7, Q),

  // Такт 12
  N(REST, E), N(G6, S), N(FS6, S), N(F6, S), N(DS6, E), N(E6, E),
  N(GS5, S), N(A5, S), N(C6, E), N(A5, S), N(C6, S), N(D6, S),

  // Такт 13
  N(REST, E), N(DS6, DE), N(D6, DE), N(C6, H),

  // Такт 14
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, E), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 15
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, S), N(E6, S), N(REST, H),

  // Такт 16
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, E), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 17
  N(E6, S), N(E6, E), N(E6, E), N(C6, S), N(E6, E), N(G6, H),

  // Такт 18
  N(C6, DE), N(G5, DE), N(E5, DE), N(A5, E), N(B5, E), N(AS5, S), N(A5, E),

  // Такт 19
  N(G5, S), N(E6, E), N(G6, S), N(A6, E), N(F6, S), N(G6, E), N(E6, E),
  N(C6, S), N(D6, S), N(B5, DE),

  // Такт 20
  N(C6, DE), N(G5, DE), N(E5, DE), N(A5, E), N(B5, E), N(AS5, S), N(A5, E),

  // Такт 21
  N(G5, S), N(E6, E), N(G6, S), N(A6, E), N(F6, S), N(G6, E), N(E6, E),
  N(C6, S), N(D6, S), N(B5, DE),

  // Такт 22
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 23
  N(B5, S), N(A6, E), N(A6, S), N(A6, S), N(G6, E), N(F6, S), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 24
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 25
  N(B5, S), N(F6, E), N(F6, S), N(F6, S), N(E6, E), N(D6, S), N(C6, H),

  // Такт 26
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 27
  N(B5, S), N(A6, E), N(A6, S), N(A6, S), N(G6, E), N(F6, S), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 28
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 29
  N(B5, S), N(F6, E), N(F6, S), N(F6, S), N(E6, E), N(D6, S), N(C6, H),

  // Такт 30
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, E), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 31
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, S), N(E6, S), N(REST, H),

  // Такт 32
  N(C6, S), N(C6, E), N(C6, E), N(C6, S), N(D6, E), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 33
  N(E6, S), N(E6, E), N(E6, E), N(C6, S), N(E6, E), N(G6, H),

  // Такт 34
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 35
  N(B5, S), N(A6, E), N(A6, S), N(A6, S), N(G6, E), N(F6, S), N(E6, S),
  N(C6, E), N(A5, S), N(G5, Q),

  // Такт 36
  N(E6, S), N(C6, E), N(G5, DE), N(GS5, E), N(A5, S),
  N(F6, E), N(F6, S), N(A5, Q),

  // Такт 37
  N(B5, S), N(F6, E), N(F6, S), N(F6, S), N(E6, E), N(D6, S), N(C6, H),
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
