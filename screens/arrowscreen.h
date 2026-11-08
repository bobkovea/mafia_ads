#pragma once

#include "../contracts/idisplay.h"
#include "../contracts/iscreen.h"
#include "../utils/flashstring.h"
	
class ArrowScreen : public IScreen
{
	public:
	ArrowScreen(IDisplay& lcd, const __FlashStringHelper* text0, const __FlashStringHelper* text1, const uint32_t typingPeriod) 
	: _lcd(lcd), 
	_text0(text0),
	_text1(text1),
	_currentText(text0),
	_baseTextTypePeriodMs(typingPeriod),
	_currentTextTypePeriodMs(typingPeriod)
	{}
	
	void OnEnter() override
	{
		_lcd.noBlink();
		_lcd.clear();
		_col = 0U;
		_byteIndex = 0U;
		_currentText = _text0;
		_lcd.createChar(0, _arrowState ? ArrowDown0 : ArrowDown1);
		_lcd.setCursor(4, 1); _lcd.write((uint8_t)0);
		_lcd.setCursor(7, 1); _lcd.write((uint8_t)0);
		_lcd.setCursor(10, 1); _lcd.write((uint8_t)0);
		_lcd.setCursor(_col, 0);
		_lcd.cursor();
		_lastTextChangeMs = _lastArrowsUpdateMs = _lastTextTypeMs = millis();
	}

	void Tick(const uint32_t now) override
	{	
		if (now - _lastArrowsUpdateMs >= ArrowsBlinkPeriodMs)
		{
			_lastArrowsUpdateMs = now;
			_lcd.createChar(0, _arrowState ? ArrowDown0 : ArrowDown1);
			_lcd.setCursor(_col, 0); // createChar почему-то сбивает положение курсора
			_arrowState = !_arrowState;
		}
		
		if (now - _lastTextChangeMs >= TextPeriodMs)
		{
			_lastTextChangeMs = now;
			_col = 0U;
			_byteIndex = 0U;
			_currentText = _currentText == _text0 ? _text1 : _text0;
			_lcd.setCursor(0, 0);
			_lcd.print(F("                "));
		}
		
		if (FlashCharAt(_currentText, _byteIndex) != 0 && (now - _lastTextTypeMs >= _currentTextTypePeriodMs))
		{
			_lastTextTypeMs = now;
			_currentTextTypePeriodMs = _baseTextTypePeriodMs + random(0, _baseTextTypePeriodMs);
			_lcd.setCursor(_col, 0);
			const uint8_t len = _lcd.printUtf8CharFlash(_currentText, _byteIndex);
			_byteIndex += len;
			++_col;
		}
	}
	
	private:
	
	IDisplay& _lcd;
	const __FlashStringHelper* _text0;
	const __FlashStringHelper* _text1;
	const __FlashStringHelper* _currentText;
	uint32_t _baseTextTypePeriodMs;
	uint32_t _currentTextTypePeriodMs;
	
	static constexpr uint32_t TextPeriodMs = 4000U;
	static constexpr uint32_t ArrowsBlinkPeriodMs = 400U;
	
	bool _arrowState = false;
	uint32_t _lastArrowsUpdateMs = 0U;
		

	uint32_t _lastTextTypeMs = 0U;
	uint8_t _col = 0U;
	uint8_t _byteIndex = 0U;
	
	uint32_t _lastTextChangeMs = 0U;

	
	uint8_t ArrowDown0[8] =
    {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b11111,
      0b01110,
      0b00100,
    };

    uint8_t ArrowDown1[8] =
    {
      0b00000,
      0b00000,
      0b00000,
      0b11111,
      0b01110,
      0b00100,
	  0b00000,
	  0b00000,
    };
};
