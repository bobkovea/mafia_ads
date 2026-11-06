#pragma once

#include "contracts/itransition.h"
#include "contracts/ibacklight.h"

class BacklightTransition : public ITransition
{
  public:
    explicit BacklightTransition(IBacklight& backlight) : _backlight(backlight) {}

    void Tick(const uint32_t now, const TransitionPhase phase) override
    {
      _backlight.TickAnalog(now, phase == TransitionPhase::FadeIn);
    }

    bool IsHidden() const override  { return _backlight.IsAnalogMinimum(); }
    bool IsVisible() const override { return _backlight.IsAnalogMaximum(); }

  private:
    IBacklight& _backlight;
};