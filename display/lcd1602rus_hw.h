#pragma once

#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include "../contracts/idisplay.h"

class Lcd1602rus_hw : public IDisplay
{
  public:
    Lcd1602rus_hw(uint8_t addr, uint8_t cols, uint8_t rows)
      : _lcd(addr, cols, rows), _cols(cols), _rows(rows)
    {}

    void begin() override
    {
      _lcd.begin(_cols, _rows);
    }

    void blink() override
    {
      _lcd.blink();
    }

    void noBlink() override
    {
      _lcd.noBlink();
    }

    void cursor() override
    {
      _lcd.cursor();
    }

    void noCursor() override
    {
      _lcd.noCursor();
    }

    void scrollDisplayRight() override
    {
      _lcd.scrollDisplayRight();
    }
    void scrollDisplayLeft() override
    {
      _lcd.scrollDisplayLeft();
    }

    void setCursor(uint8_t c, uint8_t r) override
    {
      _lcd.setCursor(c, r);
    }

    void clear() override
    {
      _lcd.clear();
    }

    void createChar(uint8_t addr, uint8_t* sym) override
    {
      _lcd.createChar(addr, sym);
    }

    void write(uint8_t addr) override
    {
      _lcd.write(addr);
    }

    void print(uint8_t num, int base) override
    {
      (void)_lcd.print(num, base);
    }

    void print(uint16_t num, int base) override
    {
      (void)_lcd.print(num, base);
    }


    void print(double num, int digits) override
    {
      (void)_lcd.print(num, digits);
    }

    // Перехватываем строку целиком
    void print(const __FlashStringHelper* str) override
	{
	  const char* p = reinterpret_cast<const char*>(str);
	  while (true)
	  {
		const uint8_t b0 = pgm_read_byte(p);
		if (b0 == 0) break;

		uint16_t code = 0;
		uint8_t  len  = 0;

		if (b0 < 0x80) { code = b0; len = 1; }
		else if ((b0 & 0xE0) == 0xC0) {
		  const uint8_t b1 = pgm_read_byte(p + 1);
		  if ((b1 & 0xC0) != 0x80) { p++; continue; }
		  code = ((uint16_t)(b0 & 0x1F) << 6) | (uint16_t)(b1 & 0x3F);
		  len = 2;
		}
		else if ((b0 & 0xF0) == 0xE0) {
		  const uint8_t b1 = pgm_read_byte(p + 1);
		  const uint8_t b2 = pgm_read_byte(p + 2);
		  if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80) { p++; continue; }
		  code = ((uint16_t)(b0 & 0x0F) << 12) |
				 ((uint16_t)(b1 & 0x3F) << 6)  |
				 (uint16_t)(b2 & 0x3F);
		  len = 3;
		}
		else { p++; continue; }

		p += len;
		printChar(code);
	  }
	}

    void printChar(uint16_t code) override
    {
      if (code < 0x80)
      {
        _lcd.write((uint8_t)code);           // ASCII как есть
      }
      else
      {
        _lcd.write(convert(code));           // кириллица -> код знакогенератора
      }
    }

    // Возвращает количество байт, которое занял символ (1..3), либо 0,
    // если строка закончилась или байт битый.
    // Удобно для машинки: печатаем по одному символу, двигая индекс.
	uint8_t printUtf8CharFlash(const __FlashStringHelper* str, uint16_t byteIndex) override
	{
	  const char* p = reinterpret_cast<const char*>(str) + byteIndex;

	  const uint8_t b0 = pgm_read_byte(p);
	  if (b0 == 0) return 0;

	  uint16_t code = 0;
	  uint8_t  len  = 0;

	  if (b0 < 0x80) { code = b0; len = 1; }
	  else if ((b0 & 0xE0) == 0xC0) {
		const uint8_t b1 = pgm_read_byte(p + 1);
		if ((b1 & 0xC0) != 0x80) return 0;
		code = ((uint16_t)(b0 & 0x1F) << 6) | (uint16_t)(b1 & 0x3F);
		len = 2;
	  }
	  else if ((b0 & 0xF0) == 0xE0) {
		const uint8_t b1 = pgm_read_byte(p + 1);
		const uint8_t b2 = pgm_read_byte(p + 2);
		if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80) return 0;
		code = ((uint16_t)(b0 & 0x0F) << 12) |
			   ((uint16_t)(b1 & 0x3F) << 6)  |
			   (uint16_t)(b2 & 0x3F);
		len = 3;
	  }
	  else return 0;

	  printChar(code);
	  return len;
	}

  private:

    LiquidCrystal_I2C _lcd;
    const uint8_t _cols;
    const uint8_t _rows;

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
