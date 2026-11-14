#pragma once

#include "../../contracts/idisplay.h"
#include "../../contracts/iscreen.h"
#include "../../utils/flashstring.h"
#include "../../timers/periodictimer.h"
#include "../customsymbols.h"

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
        _typingTimer(typingPeriodMs),
        _baseTypingPeriod(typingPeriodMs)
    {
    }

    void OnEnter(const uint32_t now) override
    {
        _lcd.noBlink();
        _lcd.clear();
        PrintArrows();
        _lcd.setCursor(0, 0);
        _lcd.cursor();
        _col = 0U;
        _byteIndex = 0U;
        _currentText = _text0;
        _arrowsBlinkTimer.Reset(now);
        _textChangeTimer.Reset(now);
        _typingTimer.Reset(now);
    }

    void Tick(const uint32_t now) override
    {
        if (_arrowsBlinkTimer.Expired(now))
        {
            ChangeArrowsState();
        }

        if (_textChangeTimer.Expired(now))
        {
            ToggleText();
            _typingTimer.Reset(now);
        }

        if (_typingTimer.Expired(now) && !TypeNextChar())
        {
            _typingTimer.SetPeriod(_baseTypingPeriod + random(0, _baseTypingPeriod));
        }
    }

  private:
    void ChangeArrowsState()
    {
        _lcd.createChar(0, _arrowState ? ArrowDown0 : ArrowDown1);
        _lcd.setCursor(_col, 0); // createChar почему-то сбивает положение курсора
        _arrowState = !_arrowState;
    }

    void ToggleText()
    {
        _col = 0U;
        _byteIndex = 0U;
        _currentText = _currentText == _text0 ? _text1 : _text0;
        _lcd.setCursor(0, 0);
        _lcd.print(F("                "));
    }

    bool TypeNextChar()
    {
        if (FlashCharAt(_currentText, _byteIndex) == 0)
            return true;
        _lcd.setCursor(_col++, 0U); // длина - не больше 16 символов!
        _byteIndex += _lcd.printUtf8CharFlash(_currentText, _byteIndex);
        return false;
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
    uint8_t _col = 0U;
    uint8_t _byteIndex = 0U;

    PeriodicTimer _arrowsBlinkTimer;
    PeriodicTimer _textChangeTimer;
    PeriodicTimer _typingTimer;
    uint32_t _baseTypingPeriod;
};
