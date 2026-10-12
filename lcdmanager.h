#pragma once

#include "lcd1602rus_hw.h"
#include "rolemanager.h"


class LcdManager
{
  public:
    LcdManager(Lcd1602rus_hw* lcd);
    void Begin();
    void ClearDisplay();
    void PrintCityFallingAsleep();
    void SetEnding(const MafiaRole role);
    void PrintIdleMessage();
    void UpdateIdleAnimation();
    void ResetIdleAnimation();
    void PrintTimeLeft(uint32_t timeLeftMs);

  private:

    static constexpr uint8_t BarLength = 16;
    static constexpr uint32_t _idleFrameTimeMs = 750;
    
    Lcd1602rus_hw* _lcd;

    uint8_t ArrowDown0[8] =
    {
      0b00000,
      0b11111,
      0b01110,
      0b00100,
      0b00000,
      0b11111,
      0b01110,
      0b00100,
    };

    uint8_t ArrowDown1[8] =
    {
      0b11111,
      0b01110,
      0b00100,
      0b00000,
      0b11111,
      0b01110,
      0b00100,
      0b00000,
    };
};
