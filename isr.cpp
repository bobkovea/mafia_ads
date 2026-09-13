#include "cardhandler.h"

extern CardHandler cardHandler;

// ============ Прерывание карты ============
ISR(INT0_vect)
{
  cardHandler.OnCardDetected();
}
