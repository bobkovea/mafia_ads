#pragma once

class IScreen
{
  public:
    virtual void OnEnter(const uint32_t now)
    {
      (void)now;
    }

    virtual void Tick(const uint32_t now)
    {
        (void)now;
    }
};