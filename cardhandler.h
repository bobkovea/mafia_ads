#pragma once
#include <SoftwareSerial.h>

// Мой:   0x42 0x00 0x9E 0x97 0x44 0x0F
// Катин: 0x4A 0x00 0x59 0x8E 0x8F 0x12

class CardHandler
{
  public:
    CardHandler(SoftwareSerial& rfid) :
      _rfid(rfid)
    {
      rfid.begin(9600);
    }

    void Prepare()
    {
      while (_rfid.available())
      {
        (void)_rfid.read();
      }
      _byteIndex = 0;
      _confirmCount = 0;
      _lastIdValid = false;
    }

    bool Handle()
    {
      bool isFound = false;

      if ((_byteIndex > 0) && ((millis() - _lastByteTime) > ByteTimeoutMs))
      {
        _byteIndex = 0;
      }

      while (_rfid.available())
      {
        const uint8_t receivedByte = _rfid.read();
        _lastByteTime = millis();

        if (receivedByte == StartByte)
        {
          _packet[0] = StartByte;
          _byteIndex = 1;
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
            idToHex();

            if (isPacketNotEmpty() && isXorCorrect())
            {
              if (_lastIdValid && isSameId())
              {
                ++_confirmCount;
              }
              else
              {
                saveId();
                _confirmCount = 1;
              }
              _lastIdValid = true;

              if (_confirmCount >= RequiredConfirmations)
              {
                _confirmCount = 0;      // сброс для следующего цикла
                _lastIdValid = false;   // чтобы не зациклиться на одном ключе
                isFound = true;

              }
            }
          }
        }
      }

      return isFound;
    }

  private:

    void idToHex()
    {
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        _id[i] = (hex2byte(_packet[1 + i * 2]) << 4) | hex2byte(_packet[2 + i * 2]);
      }
    }

    bool isPacketNotEmpty()
    {
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        if (_id[i] != 0x00) return true;
      }
      return false;
    }

    bool isXorCorrect()
    {
      uint8_t calculatedXor = 0;
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        calculatedXor ^= _id[i];
      }
      const uint8_t receivedXor = (hex2byte(_packet[11]) << 4) | hex2byte(_packet[12]);
      return calculatedXor == receivedXor;
    }

    bool isSameId()
    {
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        if (_id[i] != _prevId[i]) 
        {
          return false;
        }
      }
      return true;
    }

    // Сохраняем текущий ID как предыдущий
    void saveId()
    {
      for (uint8_t i = 0; i < IdBytesCount; ++i)
      {
        _prevId[i] = _id[i];
      }
    }

    int8_t hex2byte(uint8_t c)
    {
      if (c >= '0' && c <= '9') return c - '0';
      if (c >= 'A' && c <= 'F') return c - 'A' + 10;
      if (c >= 'a' && c <= 'f') return c - 'a' + 10;
      return -1;
    }

    static constexpr uint8_t StartByte = 0x02;
    static constexpr uint8_t EndByte = 0x03;
    static constexpr uint32_t ByteTimeoutMs = 100;
    static constexpr uint8_t PacketSize = 14;
    static constexpr uint8_t IdBytesCount = 5;
    static constexpr uint8_t RequiredConfirmations = 2;

    uint8_t _id[IdBytesCount];
    uint8_t _prevId[IdBytesCount];
    uint8_t _packet[PacketSize];
    uint8_t _byteIndex = 0;
    uint32_t _lastByteTime = 0;
    uint8_t _confirmCount = 0;
    bool _lastIdValid = false;

    SoftwareSerial& _rfid;
};
