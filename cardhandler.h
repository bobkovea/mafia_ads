#pragma once
#include <SoftwareSerial.h>

class CardHandler
{
  public:
    CardHandler(SoftwareSerial& rfid) :
      _rfid(rfid)
    {
      rfid.begin(9600);
    }

    bool Handle()
    {
      bool isFound = false;
      
      // 1. Таймаут
      if ((_byteIndex > 0) && ((millis() - _lastByteTime) > ByteTimeoutMs))
      {
        _byteIndex = 0;
      }

      while (_rfid.available())
      {
        const uint8_t receivedByte = _rfid.read();
        _lastByteTime = millis();

        // 2. Ресинхронизация по стартовому байту
        if (receivedByte == StartByte)
        {
          _packet[0] = StartByte;
          _byteIndex = 1;
          continue;
        }

        // 3. Если пакет ещё не начат — игнорируем
        if (_byteIndex == 0)
        {
          continue;
        }

        // 4. Накапливаем
        _packet[_byteIndex] = receivedByte;
        ++_byteIndex;

        // 5. Пакет собран
        if (_byteIndex == PacketSize)
        {
          _byteIndex = 0;
          if ((_packet[13] == 0x03) && isXorCorrect())
          {
            isFound = true;
          }
        }
      }

      return isFound;
    }

  private:

    bool isXorCorrect()
    {
      uint8_t data[5];
      for (int i = 0; i < 5; ++i)
      {
        data[i] = (hex2byte(_packet[1 + i * 2]) << 4) | hex2byte(_packet[2 + i * 2]);
      }

      uint8_t xor_sum = 0;
      for (int i = 0; i < 5; ++i)
      {
        xor_sum ^= data[i];
      }

      uint8_t rx_sum = (hex2byte(_packet[11]) << 4) | hex2byte(_packet[12]);

      return xor_sum == rx_sum;
    }

    // Преобразует ASCII-символ '0'..'9', 'A'..'F', 'a'..'f' в число 0..15
    int8_t hex2byte(uint8_t c)
    {
      if (c >= '0' && c <= '9') return c - '0';        // '0'..'9'  -> 0..9
      if (c >= 'A' && c <= 'F') return c - 'A' + 10;   // 'A'..'F'  -> 10..15
      if (c >= 'a' && c <= 'f') return c - 'a' + 10;   // 'a'..'f'  -> 10..15
      return -1; // невалидный символ
    }

    static constexpr uint8_t PacketSize = 14;
    static constexpr uint8_t StartByte = 0x02;
    static constexpr uint8_t EndByte = 0x03;
    static constexpr uint32_t ByteTimeoutMs = 100;
    uint8_t _packet[PacketSize];
    uint8_t _byteIndex = 0;
    uint32_t _lastByteTime = 0;
    SoftwareSerial& _rfid;
};
