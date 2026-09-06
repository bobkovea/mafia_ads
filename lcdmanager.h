#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include <GTimer.h>

struct Operation
{
  const char* msg;
};

enum class MafiaRole : uint8_t
{
  Citizen = 0,
  Sheriff = 1,
  Mafia = 2,
  Don = 3
};

class LcdManager
{
  public:
    // Конструктор
    LcdManager(LCD_1602_RUS* lcd, const Operation* operations, uint8_t count, uint8_t backlightPin = 9)
    {
      _lcd = lcd;
      _operations = operations;
      _operationsCount = count;
      _backlightPin = backlightPin;
    }

    // Инициализация
    void Begin()
    {
      _lcd->init();
      _lcd->backlight();
    }

    void ClearDisplay()
    {
      _lcd->setCursor(0, 0);
      _lcd->print("                ");
      _lcd->setCursor(0, 1);
      _lcd->print("                ");
    }

    void ResetPwm()
    {
      _pwmDirection = false;
      _brightness = 255;
      analogWrite(_backlightPin, _brightness);
    }

    void PrintCityFallingAsleep()
    {
      _lcd->setCursor(0, 0);
      _lcd->print({"   Город        "});
      _lcd->setCursor(0, 1);
      _lcd->print({"   засыпает...  "});
    }

    bool SmoothBacklightOff()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(_backlightPin, getBrightCRT(_brightness));

        _brightness -= _brightnessStep;

        if (_brightness < _minBrightness)
        {
          _brightness = _minBrightness;
        }
      }
      return _brightness == _minBrightness;
    }

    bool SmoothBacklightOn()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(_backlightPin, getBrightCRT(_brightness));

        _brightness += _brightnessStep;

        if (_brightness > _maxBrightness)
        {
          _brightness = _maxBrightness;
        }
      }
      return _brightness == _maxBrightness;
    }



    void UpdateIdle()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(_backlightPin, getBrightCRT(_brightness));

        _brightness = _pwmDirection ? _brightness + _brightnessStep : _brightness - _brightnessStep;

        if (_brightness >= _maxBrightness)
        {
          _brightness = _maxBrightness;
          _pwmDirection = false;
        }
        else if (_brightness <= _minBrightness)
        {
          _brightness = _minBrightness;
          _pwmDirection = true;
        }
      }

      EVERY_MS(_idleTextPeriodMs)
      {
        _lcd->setCursor(0, 0);
        _lcd->print("КОЛ-ВО ПОПЫТОК:");

        uint32_t var;
        EEPROM.get(0, var);
        _lcd->setCursor(0, 1);
        _lcd->print(var, 10);
      }
    }

    void UpdateOperation()
    {
      _lcd->setCursor(0, 0);
      _lcd->print(_operations[_currentOperationIndex].msg);
      ++_currentOperationIndex;
    }

    bool IsLoadingFinished()
    {
      return (_currentOperationIndex == _operationsCount) && (_currentProgressBarIndex == BarLength);
    }

    bool IsProgressBarFull()
    {
      return _currentProgressBarIndex == BarLength;
    }

    bool IsProgressBarEmpty()
    {
      return _currentProgressBarIndex == 0;
    }

    void UpdateProgressBar()
    {
      _lcd->setCursor(_currentProgressBarIndex, 1);
      _lcd->write(255);
      ++_currentProgressBarIndex;
    }

    void ResetProgressBar()
    {
      _currentProgressBarIndex = 0;
      _lcd->setCursor(0, 1);
      _lcd->print("                ");
    }

    void ResetOperations()
    {
      _currentOperationIndex = 0;
      _lcd->setCursor(0, 0);
      _lcd->print("                ");
    }

    bool UpdateLoading()
    {
      static uint32_t period = 100;
      bool loadingIsFinished = false;

      EVERY_MS(period)
      {
        // period = (uint32_t)random(50, 100); //TBD

        loadingIsFinished = IsLoadingFinished();

        if (loadingIsFinished)
        {
          ResetOperations();
          ResetProgressBar();
        }
        else
        {
          if (IsProgressBarFull())
          {
            ResetProgressBar();
            UpdateOperation();
          }
          else
          {
            UpdateProgressBar();
          }
        }
      }
      return loadingIsFinished;
    }

    void SetEnding(const MafiaRole role)
    {
      const char* endingMsg = "";
      switch (role)
      {
        case MafiaRole::Citizen:
          endingMsg = "МИРНЫЙ ЖИТЕЛЬ !";
          break;
        case MafiaRole::Sheriff:
          endingMsg = "     ШЕРИФ!     ";
          break;
        case MafiaRole::Mafia:
          endingMsg = "     МАФИЯ!    ";
          break;
        case MafiaRole::Don:
          endingMsg = "   ДОН МАФИИ!   ";
          break;

        default:
          break;
      }

      _lcd->setCursor(0, 0);
      _lcd->print({"Ваша роль:"     });

      _lcd->setCursor(0, 1);
      _lcd->print(endingMsg);
    }

  private:

    uint8_t getBrightCRT(uint8_t val)
    {
      return (uint32_t(val + 1) * (val + 1) * val) >> 16;
    }

    LCD_1602_RUS* _lcd;
    const Operation* _operations;
    uint8_t _operationsCount;
    uint8_t _currentOperationIndex = 0;
    uint8_t _currentProgressBarIndex = 0;
    static constexpr uint8_t BarLength = 16;
    uint8_t _backlightPin;

    int16_t _brightness = 255;
    int16_t _brightnessStep = 1;
    bool _pwmDirection = false;
    static constexpr uint32_t _idlePwmPeriodMs = 20;
    static constexpr uint32_t _idleTextPeriodMs = 1000;
    static constexpr uint8_t _maxBrightness = 255;
    static constexpr uint8_t _minBrightness = 25;
};
