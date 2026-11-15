#pragma once

#include "../card/cardtypes.h"

class IStorage
{
  public:
    virtual UploadStatusAndTimestamp UploadCard(const CardId& id, const uint32_t now) = 0;
};
