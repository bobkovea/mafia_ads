#pragma once

#include "../contracts/ibatterymanager.h"

template<uint8_t vbatPin, uint16_t vccMv>
class BatteryManager : public IBatteryManager
{
  public:
    void Tick(const uint32_t now) override
    {
      if (now - _vbatLastTimeMs >= VbatPeriodMs)
      {
        _vbatLastTimeMs = now;
        MeasureVbat();
      }
      //
      //      if (now - _vbatRateLastTimeMs >= VbatRatePeriodMs)
      //      {
      //        _vbatRateLastTimeMs = now;
      //        CalculateVbatRate();
      //      }
    }

    float GetVbat() override
    {
      return _vbatFiltered;
    }

    //    float GetVbatRate()
    //    {
    //      return _vbatRate;
    //    }

  private:
    static constexpr uint32_t VbatPeriodMs = 1000U;
    static constexpr float K = 0.02F;
    float _vbatFiltered = NAN;
    uint32_t _vbatLastTimeMs = 0U;

    static constexpr uint32_t VbatRatePeriodMs = 60000U;
    float _prevVbatFiltered = NAN;
    float _vbatRate = NAN;
    uint32_t _vbatRateLastTimeMs = 0U;

    void MeasureVbat()
    {
      const uint16_t adcCode = analogRead(vbatPin);
      const uint16_t vbatMv = (uint16_t)((uint32_t)adcCode * (uint32_t)vccMv / 1024U);
      const float vbat = (float)vbatMv / 1000.0F;
      FilterVbat(vbat);
    }

    void FilterVbat(float vbat)
    {
      if (isnan(_vbatFiltered))
      {
        _vbatFiltered = vbat;
        _prevVbatFiltered = vbat;
      }
      else
      {
        _vbatFiltered += (vbat - _vbatFiltered) * K;
      }
    }

    void CalculateVbatRate()
    {
      if (!isnan(_prevVbatFiltered) && !isnan(_vbatFiltered))
      {
        _vbatRate = ((_vbatFiltered - _prevVbatFiltered) / (float)VbatRatePeriodMs) * 3600000.0F; // V/h
        _prevVbatFiltered = _vbatFiltered;
      }
    }
};
