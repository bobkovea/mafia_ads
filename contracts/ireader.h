#pragma once

#include "../card/cardtypes.h"

class IReader
{
  public:
    virtual bool Read(CardId& id, const uint32_t now) = 0;
	virtual void Reset() = 0;
};
