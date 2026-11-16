#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../contracts/ibatterymanager.h"
#include "../../utils/flashstring.h"
#include "../../timers/periodictimer.h"
#include "texttyper.h"

class TypingScreen : public IScreen
{
  public:
    TypingScreen(IDisplay& lcd, const __FlashStringHelper* text, const uint32_t typingPeriodMs) :
        _lcd(lcd),
        _textTyper(lcd, text, typingPeriodMs)
    {
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.cursor();
        _lcd.clear();
        _textTyper.Reset(now);
    }

    void Tick(const uint32_t now) override
    {
        if (_textTyper.Tick(now))
        {
            _lcd.noCursor();
            _lcd.blink();
        }
    }

  private:
    IDisplay& _lcd;
    TextTyper _textTyper;
};
