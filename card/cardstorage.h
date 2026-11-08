#pragma once

#include "cardtypes.h"
#include "../contracts/istorage.h"

template <int8_t storageSize, uint32_t timeBetweenMs>
class CardStorage : public IStorage
{
  public:
	static_assert(storageSize > 0, "CardStorage size must be positive");
	
    UploadStatusAndTimestamp UploadCard(const CardId& id, const uint32_t now) override
    {
	  UploadStatus status;
		
	  // Ищем карту среди уже сохранённых.
      int8_t cardIndex = -1;
      for (int8_t i = 0; i < _cardsCount; ++i)
      {
        if (memcmp(_storage[i].id, id, IdBytesCount) == 0)
        {
          cardIndex = i;
          break;
        }
      }
		
	  // Новая карта — пробуем добавить.
      if (cardIndex < 0)
      {
		// Если есть место в базе
        if (_cardsCount < storageSize)
        {
          cardIndex = _cardsCount;
          memcpy(_storage[cardIndex].id, id, IdBytesCount);
          _storage[cardIndex].timestamp = now;
          ++_cardsCount;
		  status = UploadStatus::NewCardAdded;
        }
        else
        {
          status = UploadStatus::StorageIsFull;
        }
      }
      else
      {
		// Известная карта — проверяем интервал.       
        if (now - _storage[cardIndex].timestamp >= timeBetweenMs)
        {
          _storage[cardIndex].timestamp = now;
          status = UploadStatus::ExistingCardUpdated;
        }
		else
		{
		  status = UploadStatus::ExistingCardTimeleft;
		}
      }
	  
		return {status, _storage[cardIndex].timestamp};
    }

  private:  
	IdAndTimestamp _storage[storageSize] = {};
    uint8_t _cardsCount = 0;
};
