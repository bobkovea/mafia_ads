#pragma once

#include "../contracts/ibacklight.h"

template<uint8_t backlightPin>
class Backlight : public IBacklight
{
  public:

    void TickAnalog(const uint32_t now, const bool direction) override
    {
      if (now - _lastTickTime >= AnalogTickPeriodMs)
      {
        _lastTickTime = now;
        analogWrite(backlightPin, GetBrightCRT(_brightness));

        if (direction)
        {
          _brightness += (int16_t)BrightnessStep;
          if (_brightness > (int16_t)MaxBrightness)
          {
            _brightness = (int16_t)MaxBrightness;
          }
        }
        else
        {
          _brightness -= (int16_t)BrightnessStep;
          if (_brightness < (int16_t)MinBrightness)
          {
            _brightness = (int16_t)MinBrightness;
          }
        }
      }
    }

    bool IsAnalogMinimum() override
    {
      return _brightness == MinBrightness;
    }

    bool IsAnalogMaximum() override
    {
      return _brightness == MaxBrightness;
    }

    void TickDiscrete(const uint32_t now) override
    {
      if (_backlightTransitionsCount < _totalBacklightTransitions)
      {
        if (now - _lastTickTime >= DiscreteTickPeriodMs)
        {
          _lastTickTime = now;
          _currentPinDiscreteState = !_currentPinDiscreteState;
          digitalWrite(backlightPin, _currentPinDiscreteState);
          ++_backlightTransitionsCount;
        }
      }
    }

    void SetTotalDicsreteCycles(const uint8_t cycles) override
    {
      _totalBacklightTransitions = cycles * 2U;
    }

    bool IsDiscreteEnded() override
    {
      return _backlightTransitionsCount == _totalBacklightTransitions;
    }

    void ResetDiscrete() override
    {
      _backlightTransitionsCount = 0U;
    }

  private:

    uint8_t GetBrightCRT(uint8_t val)
    {
      return (uint32_t(val + 1) * (val + 1) * val) >> 16;
    }

    int16_t _brightness = 255;
    uint32_t _lastTickTime = 0U;
    uint8_t _backlightTransitionsCount = 0U;
    uint8_t _totalBacklightTransitions = 0U;
    bool _currentPinDiscreteState = true;

    static constexpr uint8_t MaxBrightness = 255;
    static constexpr uint8_t MinBrightness = 0;
    static constexpr uint8_t BrightnessStep = 10;
    static constexpr uint8_t AnalogTickPeriodMs = 20;
    static constexpr uint32_t DiscreteTickPeriodMs = 100U;
};
