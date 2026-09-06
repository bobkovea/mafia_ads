#include "extint.h"
#include "statemachine.h"

extern StateMachine stateMachine;

// ============ Прерывание карты ============
ISR(INT0_vect)
{
  ExtInt::DisableInterrupt();
  stateMachine.TriggerEvent(Event::CardDetected);
}
