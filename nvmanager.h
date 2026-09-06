#pragma once

#include <EEPROM.h>

class NvManager
{
  public:
    static void Initialize()
    {
      uint32_t tmp;
      EEPROM.get(AttemptsAddress, tmp);
      if (tmp == UINT32_MAX)
      {
        EEPROM.put(AttemptsAddress, (uint32_t)0);
      }
    }

    static void IncrementAttempts()
    {
      uint32_t cardsDetected;
      EEPROM.get(AttemptsAddress, cardsDetected);
      cardsDetected = (cardsDetected == UINT32_MAX) ? 0 : cardsDetected + 1;
      EEPROM.put(AttemptsAddress, cardsDetected);
    }

    static uint32_t GetAttempts()
    {
      uint32_t attempts;
      EEPROM.get(AttemptsAddress, attempts);
      return attempts;
    }
  private:

    static constexpr uint8_t AttemptsAddress = 0;
};
