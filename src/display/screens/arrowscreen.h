#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../utils/flashstring.h"
#include "../../timers/periodictimer.h"
#include "../customsymbols.h"
#include "texttyper.h"

class ArrowScreen : public IScreen
{
  public:
    ArrowScreen(
        IDisplay& lcd,
        const __FlashStringHelper* text0,
        const __FlashStringHelper* text1,
        const uint32_t typingPeriodMs,
        const uint32_t arrowsBlinkPeriodMs,
        const uint32_t textChangePeriodMs) :
        _lcd(lcd),
        _text0(text0),
        _text1(text1),
        _currentText(text0),
        _arrowsBlinkTimer(arrowsBlinkPeriodMs),
        _textChangeTimer(textChangePeriodMs),
        _textTyper(lcd, text0, typingPeriodMs) //todo add ctor with no text
    {
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.clear();
        PrintArrows();
        _lcd.cursor();
        _arrowsBlinkTimer.Reset(now);
        _textChangeTimer.Reset(now);
        _textTyper.SetText(_text0);
        _textTyper.Reset(now);
    }

    void Tick(const uint32_t now) override
    {
        if (_arrowsBlinkTimer.Expired(now))
        {
            ChangeArrowsState();
        }

        if (_textChangeTimer.Expired(now))
        {
            ClearUpperRow();
            _textTyper.SetText(_textTyper.GetText() == _text0 ? _text1 : _text0);
            _textTyper.Reset(now);
            _lcd.cursor();
        }

        if (_textTyper.Tick(now))
        {
            _lcd.noCursor();
        }
    }

  private:
    void ChangeArrowsState()
    {
        _arrowState = !_arrowState;
        _lcd.createChar(0, _arrowState ? ArrowDown0 : ArrowDown1);
        _textTyper.RestoreCursorPos(); // т.к. createChar сбивает положение курсора
    }

    void ClearUpperRow()
    {
        _lcd.setCursor(0, 0);
        _lcd.print(F("                "));
    }

    void PrintArrows()
    {
        _lcd.createChar(0, _arrowState ? ArrowDown0 : ArrowDown1);
        _lcd.setCursor(4, 1);
        _lcd.write((uint8_t)0);
        _lcd.setCursor(7, 1);
        _lcd.write((uint8_t)0);
        _lcd.setCursor(10, 1);
        _lcd.write((uint8_t)0);
    }

    IDisplay& _lcd;
    const __FlashStringHelper* _text0;
    const __FlashStringHelper* _text1;
    const __FlashStringHelper* _currentText;
    bool _arrowState = false;
    PeriodicTimer _arrowsBlinkTimer;
    PeriodicTimer _textChangeTimer;
    TextTyper _textTyper;
};
