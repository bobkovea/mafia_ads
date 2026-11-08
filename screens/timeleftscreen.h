#pragma once

#include "../contracts/idisplay.h"
#include "../contracts/iscreen.h"
#include "../utils/flashstring.h"

class TimeleftScreen : public IScreen
{
	public:
	
	TimeleftScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriod) 
	: _lcd(lcd),
	_text(text),
	_baseTextTypePeriodMs(typingPeriod),
	_currentTextTypePeriodMs(typingPeriod)
	{}
	
	void OnEnter() override
	{
		_lcd.noBlink();
		_lcd.clear();
		_col = 0U;
		_byteIndex = 0U;
		_lcd.cursor();
		_lastTimeleftUpdateMs = _lastTextTypeMs = millis();
	}

	void Tick(const uint32_t now) override
	{
		if (now - _lastTimeleftUpdateMs >= TimeleftUpdatePeriodMs)
		{
			_lastTimeleftUpdateMs = now;
			
		}
		
		if (FlashCharAt(_text, _byteIndex) != 0 && (now - _lastTextTypeMs >= _currentTextTypePeriodMs))
		{
			_lastTextTypeMs = now;
			_currentTextTypePeriodMs = _baseTextTypePeriodMs + random(0, _baseTextTypePeriodMs);
			_lcd.setCursor(_col, 0);
			const uint8_t len = _lcd.printUtf8CharFlash(_text, _byteIndex);
			_byteIndex += len;
			++_col;
		}
	}
	
	private:
	
	IDisplay& _lcd;
	const __FlashStringHelper* _text;
	uint32_t _baseTextTypePeriodMs;
	uint32_t _currentTextTypePeriodMs;
	
	static constexpr uint32_t TimeleftUpdatePeriodMs = 1000U;
	
	uint32_t _lastTimeleftUpdateMs = 0U;
		

	uint32_t _lastTextTypeMs = 0U;
	uint8_t _col = 0U;
	uint8_t _byteIndex = 0U;
};
