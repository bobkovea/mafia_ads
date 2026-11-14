#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../contracts/ibatterymanager.h"
#include "../../utils/flashstring.h"
#include "../../timers/periodictimer.h"

class TypingScreen : public IScreen
{
  public:
    TypingScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriodMs) :
        _lcd(lcd),
        _text(text),
        _baseTypingPeriod(typingPeriodMs),
        _typingTimer(typingPeriodMs)
    {
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.clear();
        _lcd.cursor();
        _typingDone = false;
        _col = _row = 0U;
        _typingTimer.Reset(now);
        _byteIndex = 0U;
    }

    void Tick(const uint32_t now) override
    {
        if (!_typingDone && _typingTimer.Expired(now))
        {
            _typingDone = TypeNextChar();
            if (_typingDone)
            {
                _lcd.noCursor();
                _lcd.blink();
            }
            _typingTimer.SetPeriod(_baseTypingPeriod + random(0, _baseTypingPeriod));
        }
    }

  private:
    bool TypeNextChar()
    {
        if (FlashCharAt(_text, _byteIndex) == 0)
            return true;
        _lcd.setCursor(_col++, _row); // длина - не больше 16 символов!
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
    uint8_t _col = 0;
    uint8_t _row = 0;
    uint8_t _byteIndex = 0;
    bool _typingDone = false;

    PeriodicTimer _typingTimer;
};
