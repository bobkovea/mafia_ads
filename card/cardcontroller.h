#pragma once

#include "cardtypes.h"
#include "cardconfirmation.h"
#include "../contracts/ireader.h"
#include "../contracts/istorage.h"

class CardController
{
  public:
    CardController(IReader& reader, IStorage& storage) :
      _reader(reader), _storage(storage)
    {
    }

    UploadStatus Poll(const uint32_t now)
    {
		UploadStatus status = UploadStatus::NoCard;
		CardId id;
		if(_reader.Read(id, now))
		{
			if(_confirmation.Confirm(id, now))
			{
				const UploadStatusAndTimestamp cardData = _storage.UploadCard(id, now);
				_lastCardTimestamp = cardData.timestamp;
				status = cardData.status;
			}
		}
		
		return status;
    }
			
    void Reset()
    {
		_reader.Reset();
		_confirmation.Reset();
	}
	
	uint32_t GetLastCardTimestamp()
	{
		return _lastCardTimestamp;
	}
   
  private:
	IReader& _reader;
	IStorage& _storage;
	uint32_t _lastCardTimestamp = 0U;
	CardConfirmation _confirmation;

};
