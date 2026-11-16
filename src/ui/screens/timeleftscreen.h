#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../contracts/ibatterymanager.h"
#include "../../timers/periodictimer.h"
#include "texttyper.h"

class TimeleftScreen : public IScreen
{
  public:
    TimeleftScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriodMs) :
        _lcd(lcd),
        _textTyper(lcd, text, typingPeriodMs),
        _timeleftUpdateTimer(MsInOneSecond)
    {
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.cursor();
        _lcd.clear();
        _timeleftUpdateTimer.Reset(now);
        _textTyper.Reset(now);
    }

    void Tick(const uint32_t now) override
    {
        if (_textTyper.IsDone() && _timeleftUpdateTimer.Expired(now))
        {
            const int32_t timeLeftMs = MsIn24h - ((int32_t)now - (int32_t)_cardTimestampMs);
            _lcd.setCursor(4, 1);
            PrintTimeLeft(timeLeftMs < 0 ? 0U : (uint32_t)timeLeftMs);
        }

        if (_textTyper.Tick(now))
        {
            _lcd.noCursor();
        }
    }

    void SetCardTimestamp(const uint32_t timestampMs)
    {
        _cardTimestampMs = timestampMs;
    }

  private:
    void PrintTimeLeft(const uint32_t timeLeftMs)
    {
        const uint32_t totalSeconds = timeLeftMs / 1000U;
        const uint8_t hours = (uint8_t)(totalSeconds / 3600U);
        const uint8_t minutes = (uint8_t)((totalSeconds % 3600U) / 60U);
        const uint8_t seconds = (uint8_t)(totalSeconds % 60U);

        PrintTwoDigits(hours);
        PrintDelimiter();
        PrintTwoDigits(minutes);
        PrintDelimiter();
        PrintTwoDigits(seconds);
    }

    void PrintTwoDigits(uint8_t value)
    {
        _lcd.printChar((char)('0' + value / 10));
        _lcd.printChar((char)('0' + value % 10));
    }

    void PrintDelimiter()
    {
        _lcd.printChar(':');
    }

    IDisplay& _lcd;
    TextTyper _textTyper;
    PeriodicTimer _timeleftUpdateTimer;
    uint32_t _cardTimestampMs = 0U;
    static constexpr int32_t MsIn24h = 86400000U; // 24 часа
    static constexpr uint32_t MsInOneSecond = 1000U;
};
