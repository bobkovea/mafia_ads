#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../contracts/ibatterymanager.h"
#include "../../timers/periodictimer.h"

class TimeleftScreen : public IScreen
{
  public:
    TimeleftScreen(
        IDisplay& lcd,
        const __FlashStringHelper* text,
        const uint32_t typingPeriodMs,
        const uint32_t timeleftPeriodMs) :
        _lcd(lcd),
        _text(text),
        _timeleftUpdateTimer(timeleftPeriodMs),
        _typingTimer(typingPeriodMs),
        _baseTypingPeriod(typingPeriodMs)
    {
    }

    void SetCardTimestamp(const uint32_t timestampMs)
    {
        _cardTimestampMs = timestampMs;
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.clear();
        _lcd.cursor();
        _col = _row = 0U;
        _byteIndex = 0U;
        _typingDone = false;
        _timeleftUpdateTimer.Reset(now);
        _typingTimer.Reset(now);
    }

    void Tick(const uint32_t now) override
    {
        if (_typingDone && _timeleftUpdateTimer.Expired(now))
        {
            const int32_t timeLeftMs = Time24hMs - ((int32_t)now - (int32_t)_cardTimestampMs);
            PrintTimeLeft(timeLeftMs < 0 ? 0U : (uint32_t)timeLeftMs);
        }

        if (!_typingDone && _typingTimer.Expired(now))
        {
            _typingDone = TypeNextChar();
            if (_typingDone)
            {
                _lcd.noCursor();
            }
            _typingTimer.SetPeriod(_baseTypingPeriod + random(0, _baseTypingPeriod));
        }
    }

  private:
    bool TypeNextChar()
    {
        if (FlashCharAt(_text, _byteIndex) == 0)
            return true;
        _lcd.setCursor(_col++, _row);
        _byteIndex += _lcd.printUtf8CharFlash(_text, _byteIndex);
        if (_col >= 16)
        {
            _col = 0;
            ++_row;
        }
        return false;
    }

    void PrintTimeLeft(const uint32_t timeLeftMs)
    {
        const uint32_t totalSeconds = timeLeftMs / 1000U;
        const uint8_t hours = (uint8_t)(totalSeconds / 3600U);
        const uint8_t minutes = (uint8_t)((totalSeconds % 3600U) / 60U);
        const uint8_t seconds = (uint8_t)(totalSeconds % 60U);

        _lcd.setCursor(6, 1);
        if (hours < 10)
            _lcd.printChar('0');
        _lcd.print(hours, 10);
        _lcd.printChar(':');
        if (minutes < 10)
            _lcd.printChar('0');
        _lcd.print(minutes, 10);
        _lcd.printChar(':');
        if (seconds < 10)
            _lcd.printChar('0');
        _lcd.print(seconds, 10);
    }

    IDisplay& _lcd;
    const __FlashStringHelper* _text;
    uint8_t _col = 0U;
    uint8_t _row = 0U;
    uint8_t _byteIndex = 0U;
    uint32_t _cardTimestampMs = 0U;
    bool _typingDone = false;
    static constexpr int32_t Time24hMs = 86400000U; // 24 часа
    PeriodicTimer _timeleftUpdateTimer;
    PeriodicTimer _typingTimer;
    uint32_t _baseTypingPeriod;
};
