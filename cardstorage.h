#pragma once

#include "cardhandler.h"

// Мой:   0x42 0x00 0x9E 0x97 0x44
// Катин: 0x4A 0x00 0x59 0x8E 0x8F

struct Card
{
  uint8_t id[5];
  uint32_t timestamp;
};

class CardStorage
{
  public:

    uint32_t AddId(const uint8_t* id)
    {
      const uint32_t now = millis();
      
      uint8_t cardIndex = 0xFF;
      for (uint8_t i = 0; i < _cardsCount; ++i)
      {
        if (memcmp(storage[i].id, id, 5) == 0)
        {
          cardIndex = i;
          break;
        }
      }

      if (cardIndex == 0xFF)
      {
        if (_cardsCount < StorageSize)
        {
          cardIndex = _cardsCount;
          memcpy(storage[cardIndex].id, id, 5);
          storage[cardIndex].timestamp = now;
          ++_cardsCount;
          return 0U;
        }
        else
        {
          // TODO если заполнилось, что делать?
          return 0U;
        }
      }
      else
      {
        const uint32_t elapsed = now - storage[cardIndex].timestamp;
        
        if (elapsed >= TimeBetweenMs)
        {
          storage[cardIndex].timestamp = now;
          return 0U;
        }
        else
        {
          return TimeBetweenMs - elapsed;
        }
      }
    }

  private:
    static constexpr uint8_t StorageSize = 32;
    Card storage[StorageSize] = {};
    uint8_t _cardsCount = 0;
    static constexpr uint32_t TimeBetweenMs = 86400000U;

};
