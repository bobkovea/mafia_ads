#pragma once

#include "../contracts/idisplay.h"
#include "../contracts/iscreen.h"
#include "../utils/flashstring.h"

class TypingScreen : public IScreen
{
	public:
	TypingScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriod) :
	_lcd(lcd),
	_text(text), 
	_baseTextTypePeriodMs(typingPeriod),
	_currentTextTypePeriodMs(typingPeriod)
	{
	}
	
	void OnEnter() override
	{
		_isFinished = false;
		_col = 0U;
		_row = 0U;
		_byteIndex = 0U;
		_lcd.noBlink();
		_lcd.clear();
		_lcd.setCursor(0, 0);
		_lcd.cursor();
	}

	void Tick(const uint32_t now) override
	{
		if (!_isFinished && (now - _lastUptimeMs >= _currentTextTypePeriodMs))
		{
			_lastUptimeMs = now;
			
			// Если предыдущий тик был паузой — вот теперь очищаем экран
			if (_currentTextTypePeriodMs == PauseAfterFilledScreenMs)
			{
				_lcd.clear();
			}
			
			_currentTextTypePeriodMs = _baseTextTypePeriodMs + random(0, _baseTextTypePeriodMs);
			_lcd.setCursor(_col, _row);
			const uint8_t len = _lcd.printUtf8CharFlash(_text, _byteIndex);
			_byteIndex += len;
			++_col;
			if (_col >= 16)
			{
				_col = 0;
				if(_row == 1)
				{   // Экран заполнен: не чистим, просто "засыпаем" на длинный период
					_row = 0;
					_currentTextTypePeriodMs = PauseAfterFilledScreenMs;
				}
				else
				{
					_row = 1;
				}
			}
			
			if(FlashCharAt(_text, _byteIndex) == 0)
			{
				_isFinished = true;
				_lcd.noCursor();
				_lcd.blink();
			}
		}
	}
	
	private:

	IDisplay& _lcd;
	const __FlashStringHelper* _text;
	uint32_t _baseTextTypePeriodMs;
	uint32_t _currentTextTypePeriodMs;
	
	uint32_t _lastUptimeMs = 0U;
	uint8_t _col = 0;
	uint8_t _row = 0;
	uint8_t _byteIndex = 0;
	bool _isFinished = false;
	static constexpr uint32_t PauseAfterFilledScreenMs = 1000U;
};
