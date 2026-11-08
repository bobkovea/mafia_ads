#pragma once

#include "cardtypes.h"

class CardConfirmation
{
public:

    bool Confirm(const CardId& id, uint32_t now)
	{
		bool isConfirmed = false;

		// Сброс, если прошло слишком много времени
		if (now - _lastConfirmTime >= TimeoutMs)
		{
			_seen = false;
		}

		if (_seen && memcmp(_prevId, id, IdBytesCount) == 0)
		{
			// Второе чтение той же карты — подтверждено
			_seen = false;
			isConfirmed = true;
		}
		else
		{
			// Первое чтение или другая карта — начинаем заново
			memcpy(_prevId, id, IdBytesCount);
			_seen = true;
		}

		_lastConfirmTime = now;
		return isConfirmed;
	}
	
    void Reset()
	{
		_seen = false;
	}
	
private:
	static constexpr uint32_t TimeoutMs = 10000U; //todo
	bool _seen = false;
	uint32_t _lastConfirmTime = 0U;
	CardId _prevId = {};
};