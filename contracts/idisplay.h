#pragma once

class IDisplay
{
  public:
    virtual void begin() = 0;
    virtual void setCursor(uint8_t c, uint8_t r) = 0;
    virtual void clear() = 0;
	virtual void createChar(uint8_t addr, uint8_t* sym) = 0;
	virtual void write(uint8_t addr) = 0;
    virtual void print(uint8_t num, int base = 10) = 0;
    virtual void print(uint16_t num, int base = 10) = 0;
    virtual void print(double num, int digits = 2) = 0;
	virtual void print(const __FlashStringHelper* str) = 0;
	virtual uint8_t printUtf8CharFlash(const __FlashStringHelper* str, uint16_t byteIndex) = 0;
	virtual void printChar(uint16_t code) = 0;;
	virtual void blink() = 0;
	virtual void noBlink() = 0;
	virtual void cursor() = 0;
	virtual void noCursor() = 0;
	virtual void scrollDisplayRight() = 0;
	virtual void scrollDisplayLeft() = 0;
	

};
