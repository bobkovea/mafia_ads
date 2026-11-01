#pragma once

#include "contracts/itransition.h"
#include "contracts/idisplay.h"

class ShiftTransition : public ITransition
{
  public:
    explicit ShiftTransition(IDisplay& lcd) : _lcd(lcd) {}

    void Start(const uint32_t now) override
    {
      _isFinished = false;
      _scrollsCount = 0;
      _lastShiftMs = now;
    }

    void Tick(const uint32_t now, const TransitionPhase phase) override
    {
      if (!_isFinished)
      {
        if (now - _lastShiftMs >= ShiftPeriodMs)
        {
          _lastShiftMs = now;
          _lcd.scrollDisplayLeft();
          ++_scrollsCount;
          if (_scrollsCount >= 16)
          {
            _isFinished = true;
            _lcd.clear(); // +setCursor(0, 0)
          }
        }
      }
    }

    bool IsHidden() const override  {
      return _isFinished;
    }
    bool IsVisible() const override {
      return _isFinished;
    }

  private:
    IDisplay& _lcd;
    static constexpr uint32_t ShiftPeriodMs = 50U;
    uint8_t _scrollsCount = 0U;
    uint32_t _lastShiftMs = 0U;
    bool _isFinished = false;
};
