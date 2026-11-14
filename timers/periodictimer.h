#pragma once

class PeriodicTimer
{
  public:
    constexpr explicit PeriodicTimer(uint32_t periodMs = 0) :
        _period(periodMs)
    {
    }

    void SetPeriod(uint32_t periodMs)
    {
        _period = periodMs;
    }

    void Reset(uint32_t now)
    {
        _last = now;
    }

    // Сдвигаем на now, а не на _last+_period — чтобы не «догонять» при пропущенных тиках
    void Restart(uint32_t now)
    {
        _last = now;
    }

    bool Expired(uint32_t now)
    {
        if (now - _last < _period)
            return false;
        _last = now;
        return true;
    }

  private:
    uint32_t _period;
    uint32_t _last = 0;
};