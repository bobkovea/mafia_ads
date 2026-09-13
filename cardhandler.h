#pragma once

#include <Arduino.h>
#include "extint.h"
#include "statemachine.h"

extern StateMachine stateMachine;

class CardHandler
{
  public:

    void OnCardDetected()
    {
      _timer = millis();

      if (_timer - millis() < MaxTimer)
      {
        ++_detectedCounter;

        if (_detectedCounter >= MaxDetectedCounter)
        {
          _detectedCounter = 0;
          ExtInt::DisableInterrupt();
          stateMachine.TriggerEvent(Event::CardDetected);
        }
      }
      else
      {
        _detectedCounter = 1;
      }
    }

  private:
    uint8_t _detectedCounter = 0;
    uint32_t _timer = 0;
    static constexpr uint8_t MaxDetectedCounter = 3;
    static constexpr uint8_t MaxTimer = 100;
};
