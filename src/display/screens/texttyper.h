#pragma once

#include "../../contracts/idisplay.h"
#include "../../utils/flashstring.h"
#include "../../timers/periodictimer.h"

class TextTyper
{
  public:
    TextTyper(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriodMs) :
        _lcd(lcd),
        _text(text),
        _baseTypingPeriod(typingPeriodMs),
        _typingTimer(typingPeriodMs)
    {
    }

    // Продвигает печать. Возвращает true, если в этот тик печать завершилась.
    bool Tick(const uint32_t now)
    {
        if (_typingDone || !_typingTimer.Expired(now))
            return false;

        if (TypeNextChar())
        {
            _typingDone = true;
            _typingTimer.SetPeriod(_baseTypingPeriod);
            return true; // завершились именно в этом тике
        }
        _typingTimer.SetPeriod(_baseTypingPeriod + random(0, _baseTypingPeriod));
        return false;
    }

    bool IsDone() const
    {
        return _typingDone;
    }

     void SetText(const __FlashStringHelper* text)
    {
        _text = text;
    }

    const __FlashStringHelper* GetText()
    {
        return _text;
    }


    void Reset(const uint32_t now)
    {
        _col = _row = _byteIndex = 0U;
        _typingDone = false;
        _lcd.setCursor(0, 0);
        _typingTimer.Reset(now);
    }


    void RestoreCursorPos()
    {
        _lcd.setCursor(_col, _row);
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

    IDisplay& _lcd;
    const __FlashStringHelper* _text;
    uint32_t _baseTypingPeriod;
    uint8_t _col = 0U, _row = 0U, _byteIndex = 0U;
    bool _typingDone = false;
    PeriodicTimer _typingTimer;
};