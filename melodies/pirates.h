
#pragma once

#include "melodybase.h"

namespace Pirates
{

static constexpr uint16_t Tempo = 1050U;
#undef N
#define N(note, dur) CreateNote(note, dur, Tempo)

const Note melody[] PROGMEM =
{

	/*N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(F5, E), N(G5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(D5, E), N(C5, E), N(D5, DQ), N(REST, E),

	N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(F5, E), N(G5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(D5, E), N(C5, E), N(D5, DQ), N(REST, E),

	N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(F5, E), N(G5, Q), N(G5, E), N(REST, E),
	N(G5, E), N(A5, E), N(AS5, Q), N(AS5, E), N(REST, E),
	N(A5, E), N(G5, E), N(A5, E), N(D5, Q), N(REST, E),

	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(G5, Q), N(A5, E), N(D5, Q), N(REST, E),
	N(D5, E), N(F5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(F5, E), N(D5, E), N(E5, DQ), N(REST, DQ),

  N(D5, Q), N(D5, E),*/
  //Repeat of first part
	N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(F5, E), N(G5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(D5, E), N(C5, E), N(D5, DQ), N(REST, E),

	N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(F5, E), N(G5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(D5, E), N(C5, E), N(D5, DQ), N(REST, E),

	N(A4, E), N(C5, E), N(D5, Q), N(D5, E), N(REST, E),
	N(D5, E), N(F5, E), N(G5, Q), N(G5, E), N(REST, E),
	N(G5, E), N(A5, E), N(AS5, Q), N(AS5, E), N(REST, E),
	N(A5, E), N(G5, E), N(A5, E), N(D5, Q), N(REST, E),

	N(D5, E), N(E5, E), N(F5, Q), N(F5, E), N(REST, E),
	N(G5, Q), N(A5, E), N(D5, Q), N(REST, E),
	N(D5, E), N(F5, E), N(E5, Q), N(E5, E), N(REST, E),
	N(F5, E), N(D5, E), N(E5, DQ), N(REST, DQ),
	//End of Repeat

	N(A5, Q), N(REST, E), N(REST, DQ), N(AS5, Q), N(REST, E), N(REST, DQ),
	N(A5, E), N(A5, E), N(REST, E), N(C6, E), N(REST, E), N(A5, E), N(G5, E), N(REST, E), N(REST, DQ),
	N(G5, Q), N(REST, E), N(REST, DQ), N(F5, Q), N(REST, E), N(REST, DQ),
	N(E5, E), N(F5, E), N(REST, E), N(E5, E), N(REST, E), N(D5, H),

	N(A5, Q), N(REST, E), N(REST, DQ), N(AS5, Q), N(REST, E), N(REST, DQ),
	N(A5, E), N(A5, E), N(REST, E), N(C6, E), N(REST, E), N(A5, E), N(G5, E), N(REST, E), N(REST, DQ),
	N(G5, Q), N(REST, E), N(REST, DQ), N(F5, Q), N(REST, E), N(REST, DQ),
	N(E5, E), N(F5, E), N(REST, E), N(E5, E), N(REST, E), N(D5, H)
};

const uint16_t melodyLength = sizeof(melody) / sizeof(melody[0]);

}
