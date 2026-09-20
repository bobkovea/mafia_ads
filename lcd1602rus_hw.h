#pragma once

#include <LiquidCrystal_I2C.h>

class Lcd1602rus_hw : public LiquidCrystal_I2C
{
  public:
    Lcd1602rus_hw(uint8_t addr, uint8_t cols, uint8_t rows)
      : LiquidCrystal_I2C(addr, cols, rows)
    {}

    // Перехватываем строку целиком
    void print(const char* str)
    {
      while (*str)
      {
        uint16_t code = 0;

        if ((uint8_t)*str < 0x80)
        {
          // ASCII
          code = (uint8_t) * str++;
          LiquidCrystal_I2C::write((uint8_t)code);
          continue;
        }
        else if (((uint8_t)*str & 0xE0) == 0xC0)
        {
          // 2-байтовый UTF-8
          code = ((uint16_t)(*str & 0x1F) << 6) |
                 (uint16_t)(*(str + 1) & 0x3F);
          str += 2;
        }
        else if (((uint8_t)*str & 0xF0) == 0xE0)
        {
          // 3-байтовый UTF-8
          code = ((uint16_t)(*str & 0x0F) << 12) |
                 ((uint16_t)(*(str + 1) & 0x3F) << 6) |
                 (uint16_t)(*(str + 2) & 0x3F);
          str += 3;
        }
        else
        {
          str++; continue; // битый байт — пропускаем
        }

        LiquidCrystal_I2C::write(convert(code));
      }
    }

  private:

    uint8_t convert(const uint16_t utf16)
    {
      switch (utf16)
      {
        // ===== Заглавные буквы =====
        case 0x0410: return 0x41; // А
        case 0x0411: return 0xA0; // Б
        case 0x0412: return 0x42; // В
        case 0x0413: return 0xA1; // Г
        case 0x0414: return 0xE0; // Д
        case 0x0415: return 0x45; // Е
        case 0x0401: return 0xA2; // Ё
        case 0x0416: return 0xA3; // Ж
        case 0x0417: return 0xA4; // З
        case 0x0418: return 0xA5; // И
        case 0x0419: return 0xA6; // Й
        case 0x041A: return 0x4B; // К
        case 0x041B: return 0xA7; // Л
        case 0x041C: return 0x4D; // М
        case 0x041D: return 0x48; // Н
        case 0x041E: return 0x4F; // О
        case 0x041F: return 0xA8; // П
        case 0x0420: return 0x50; // Р
        case 0x0421: return 0x43; // С
        case 0x0422: return 0x54; // Т
        case 0x0423: return 0xA9; // У
        case 0x0424: return 0xAA; // Ф
        case 0x0425: return 0x58; // Х
        case 0x0426: return 0xE1; // Ц
        case 0x0427: return 0xAB; // Ч
        case 0x0428: return 0xAC; // Ш
        case 0x0429: return 0xE2; // Щ
        case 0x042A: return 0xAD; // Ъ
        case 0x042B: return 0xAE; // Ы
        case 0x042C: return 0x62; // Ь
        case 0x042D: return 0xAF; // Э
        case 0x042E: return 0xB0; // Ю
        case 0x042F: return 0xB1; // Я

        // ===== Строчные буквы =====
        case 0x0430: return 0x61; // а
        case 0x0431: return 0xB2; // б
        case 0x0432: return 0xB3; // в
        case 0x0433: return 0xB4; // г
        case 0x0434: return 0xE3; // д
        case 0x0435: return 0x65; // е
        case 0x0451: return 0xB5; // ё
        case 0x0436: return 0xB6; // ж
        case 0x0437: return 0xB7; // з
        case 0x0438: return 0xB8; // и
        case 0x0439: return 0xB9; // й
        case 0x043A: return 0xBA; // к
        case 0x043B: return 0xBB; // л
        case 0x043C: return 0xBC; // м
        case 0x043D: return 0xBD; // н
        case 0x043E: return 0x6F; // о
        case 0x043F: return 0xBE; // п
        case 0x0440: return 0x70; // р
        case 0x0441: return 0x63; // с
        case 0x0442: return 0xBF; // т
        case 0x0443: return 0x79; // у
        case 0x0444: return 0xE4; // ф
        case 0x0445: return 0x78; // х
        case 0x0446: return 0xE5; // ц
        case 0x0447: return 0xC0; // ч
        case 0x0448: return 0xC1; // ш
        case 0x0449: return 0xE6; // щ
        case 0x044A: return 0xC2; // ъ
        case 0x044B: return 0xC3; // ы
        case 0x044C: return 0xC4; // ь
        case 0x044D: return 0xC5; // э
        case 0x044E: return 0xC6; // ю
        case 0x044F: return 0xC7; // я
        default: return '?';      // неизвестный символ
      }
    }
};
