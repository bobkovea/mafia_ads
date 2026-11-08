#pragma once

#ifndef FPSTR
#define FPSTR(p) (reinterpret_cast<const __FlashStringHelper*>(p))
#endif

inline char FlashCharAt(const __FlashStringHelper* s, uint16_t index)
{
  return (char)pgm_read_byte(reinterpret_cast<const char*>(s) + index);
}