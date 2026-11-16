#pragma once

#include "../../contracts/itransition.h"

class DummyTransition : public ITransition
{
  public:
    void Tick(const uint32_t now, const TransitionPhase phase) override
    {
       (void)now;
       (void)phase;
    }

    bool IsHidden() const override
    {
        return true;
    }

    bool IsVisible() const override
    {
        return true;
    }
};