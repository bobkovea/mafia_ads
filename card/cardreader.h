#pragma once

#include <SoftwareSerial.h>
#include "cardtypes.h"
#include "../contracts/ireader.h"

class CardReader : public IReader
{
  public:
    CardReader(SoftwareSerial& rfid) :
      _rfid(rfid)
    {
    }

    void Reset() override
    {
      while (_rfid.available())
      {
        (void)_rfid.read();
      }
	  
      _byteIndex = 0;
    }
    
    bool Read(CardId& id, const uint32_t now) override
    {
	  bool cardIsFound = false;
		
      if ((_byteIndex > 0) && (now - _lastByteTime > PackageTimeoutMs))
      {
        _byteIndex = 0;
      }

      while (_rfid.available())
      {
        const uint8_t receivedByte = _rfid.read();

        if (receivedByte == StartByte)
        {
          _packet[0] = StartByte;
          _byteIndex = 1;
		  _lastByteTime = now;
          continue;
        }

        if (_byteIndex == 0)
        {
          continue;
        }

        _packet[_byteIndex++] = receivedByte;

        if (_byteIndex == PacketSize)
        {
          _byteIndex = 0;

          if (_packet[13] == EndByte)
          {
            Id2Bytes(id);
			cardIsFound = IsXorCorrect(id);
		  }
		}
      }

      return cardIsFound;
    }

  private:

    void Id2Bytes(CardId& id)
    {
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        id[i] = (Hex2byte(_packet[1 + i * 2]) << 4) | Hex2byte(_packet[2 + i * 2]);
      }
    }

    bool IsXorCorrect(const CardId& id)
    {
      uint8_t calculatedXor = 0;
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        calculatedXor ^= id[i];
      }
      const uint8_t receivedXor = (Hex2byte(_packet[11]) << 4) | Hex2byte(_packet[12]);
      return calculatedXor == receivedXor;
    }

    int8_t Hex2byte(uint8_t c)
    {
      if (c >= '0' && c <= '9') return c - '0';
      if (c >= 'A' && c <= 'F') return c - 'A' + 10;
      if (c >= 'a' && c <= 'f') return c - 'a' + 10;
      return -1;
    }

    static constexpr uint8_t StartByte = 0x02;
    static constexpr uint8_t EndByte = 0x03;
    static constexpr uint32_t PackageTimeoutMs = 200U;
    static constexpr uint8_t PacketSize = 14;

    uint8_t _packet[PacketSize];
    uint8_t _byteIndex = 0;
    uint32_t _lastByteTime = 0;

    SoftwareSerial& _rfid;
};
