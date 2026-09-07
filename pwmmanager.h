#pragma once

#include <GTimer.h>

template<uint8_t backlightPin>
class PwmManager
{
  public:

    void ResetPwm()
    {
      _pwmDirection = false;
      _brightness = 255;
      analogWrite(backlightPin, _brightness);
    }

    bool SmoothBacklightOff()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(backlightPin, GetBrightCRT(_brightness));

        _brightness -= _brightnessStep;

        if (_brightness < _minBrightness)
        {
          _brightness = _minBrightness;
        }
      }
      return _brightness == _minBrightness;
    }

    bool SmoothBacklightOn()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(backlightPin, GetBrightCRT(_brightness));

        _brightness += _brightnessStep;

        if (_brightness > _maxBrightness)
        {
          _brightness = _maxBrightness;
        }
      }
      return _brightness == _maxBrightness;
    }

    void Breath()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(backlightPin, GetBrightCRT(_brightness));

        _brightness = _pwmDirection ? _brightness + _brightnessStep : _brightness - _brightnessStep;

        if (_brightness >= _maxBrightness)
        {
          _brightness = _maxBrightness;
          _pwmDirection = false;
        }
        else if (_brightness <= _minBrightness)
        {
          _brightness = _minBrightness;
          _pwmDirection = true;
        }
      }
    }

  private:

    uint8_t GetBrightCRT(uint8_t val)
    {
      return (uint32_t(val + 1) * (val + 1) * val) >> 16;
    }

    static constexpr uint32_t _idlePwmPeriodMs = 20;
    static constexpr uint8_t _maxBrightness = 255;
    static constexpr uint8_t _minBrightness = 25;

    int16_t _brightness = 255;
    int16_t _brightnessStep = 1;
    bool _pwmDirection = false;
};
