#pragma once

class IBacklight
{
  public:
    virtual void TickAnalog(const uint32_t now, const bool direction) = 0;
    virtual bool IsAnalogMinimum() = 0;
    virtual bool IsAnalogMaximum() = 0;

    virtual void TickDiscrete(const uint32_t now) = 0;
    virtual void ResetDiscrete() = 0;
    virtual void SetTotalDicsreteCycles(const uint8_t cycles) = 0;
    virtual bool IsDiscreteEnded() = 0;
};
