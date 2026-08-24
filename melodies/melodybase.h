#pragma once
#include "notes.h"

#include "stdint.h"

template<uint16_t baseTime>
struct Duration
{
  static constexpr uint16_t W = baseTime;
  static constexpr uint16_t H = W / 2;
  static constexpr uint16_t Q = W / 4;
  static constexpr uint16_t E = W / 8;
  static constexpr uint16_t S = W / 16;
  static constexpr uint16_t DE = E + S;
  static constexpr uint16_t DQ = Q + E;
  static constexpr uint16_t DH = H + Q;
};

#define CreateNote(pitch, duration, baseTime) { NOTE_##pitch, Duration<baseTime>::duration }