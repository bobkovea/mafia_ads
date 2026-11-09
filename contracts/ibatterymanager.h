#pragma once

class IBatteryManager {
public:
  virtual void Tick(const uint32_t now) = 0;
  virtual float GetVbat() = 0;
};