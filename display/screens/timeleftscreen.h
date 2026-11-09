#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../contracts/ibatterymanager.h"

class TimeleftScreen : public IScreen
{
	public:
	
	TimeleftScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriod) 
	: _lcd(lcd),
	_text(text),
	_baseTextTypePeriodMs(typingPeriod),
	_currentTextTypePeriodMs(typingPeriod)
	{}
	
	void SetCardTimestamp(const uint32_t timestampMs)
	{
		_cardTimestampMs = timestampMs;
	}

	void OnEnter() override
	{
		_lcd.noBlink();
		_lcd.noCursor();
		_lcd.clear();
		_col = _row = 0U;
		_byteIndex = 0U;
		_lastTimeleftUpdateMs = _lastTextTypeMs = 0U;
	}

	void Tick(const uint32_t now) override
	{
		if (now - _lastTimeleftUpdateMs >= TimeleftUpdatePeriodMs)
		{
			_lastTimeleftUpdateMs = now;
			const int32_t timeLeftMs = Time24hMs - ((int32_t)now - (int32_t)_cardTimestampMs);
			PrintTimeLeft(timeLeftMs < 0 ? 0U : (uint32_t)timeLeftMs);
		}
		
		if (FlashCharAt(_text, _byteIndex) != 0 && (now - _lastTextTypeMs >= _currentTextTypePeriodMs))
		{
			_lastTextTypeMs = now;
			_currentTextTypePeriodMs = _baseTextTypePeriodMs + random(0, _baseTextTypePeriodMs);
			_lcd.setCursor(_col, _row);
			const uint8_t len = _lcd.printUtf8CharFlash(_text, _byteIndex);
			_byteIndex += len;
			++_col;
			if (_col >= 16)
			{
				_col = 0;
				_row = _row == 0 ? 1 : 0;
			}
		}
	}
	
	private:
	
	IDisplay& _lcd;
	const __FlashStringHelper* _text;
	uint32_t _baseTextTypePeriodMs;
	uint32_t _currentTextTypePeriodMs;
	
	static constexpr uint32_t TimeleftUpdatePeriodMs = 1000U;
	
	uint32_t _lastTimeleftUpdateMs = 0U;
	uint32_t _cardTimestampMs = 0U;

	uint32_t _lastTextTypeMs = 0U;
	uint8_t _col = 0U;
	uint8_t _row = 0U;
	uint8_t _byteIndex = 0U;

	static constexpr int32_t Time24hMs = 86400000U; // 24 часа

	void PrintTimeLeft(const uint32_t timeLeftMs)
	{
		const uint32_t totalSeconds = timeLeftMs / 1000U;
		const uint8_t hours = (uint8_t)(totalSeconds / 3600U);
		const uint8_t minutes = (uint8_t)((totalSeconds % 3600U) / 60U);
		const uint8_t seconds = (uint8_t)(totalSeconds % 60U);

		_lcd.setCursor(6, 1);
		if (hours < 10) _lcd.printChar('0');
		_lcd.print(hours, 10);
		_lcd.printChar(':');
		if (minutes < 10) _lcd.printChar('0');
		_lcd.print(minutes, 10);
		_lcd.printChar(':');
		if (seconds < 10) _lcd.printChar('0');
		_lcd.print(seconds, 10);
	}
};
