#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include <GTimer.h>

// Структура операции
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

struct Ending
{

};

// Класс для управления LCD с прогресс-баром
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

    void UpdateIdle()
    {
      EVERY_MS(_idlePwmPeriodMs)
      {
        analogWrite(_backlightPin, _brightness);

        _brightness = _pwmDirection ? _brightness + _brightnessStep : _brightness - _brightnessStep;

        if (_brightness >= 255)
        {
          _brightness = 255;
          _pwmDirection = false;
        }
        else if (_brightness <= 0)
        {
          _brightness = 0;
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

      EVERY_MS(period)
      {
        // period = (uint32_t)random(50, 100); //TBD

        const bool loadingIsFinished = IsLoadingFinished(); 
        
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

        return loadingIsFinished;
      }
    }
    /*
        void SetEnding(const MafiaRole role)
        {
          _currentEnding = role;
        }

        void UpdateEnding()
        {
          const char* endingMsg = "";
          switch (_currentEnding)
          {
            case MafiaRole::Citizen:
              endingMsg = "Citizens Win!";
              break;
            case MafiaRole::Sheriff:
              endingMsg = "Sheriff Wins!";
              break;
            case MafiaRole::Mafia:
              endingMsg = "Sheriff Wins!";
              break;
            case MafiaRole::Don:
              endingMsg = "Don Wins!";
              break;

            default:
              break;
          }

          EVERY_MS(1000)
          {
            static uint8_t i = 0;
            _lcd->setCursor(i++, 0);
            _lcd->print(endingMsg);
          }
        }
    */

  private:
    LCD_1602_RUS* _lcd;
    const Operation* _operations;
    uint8_t _operationsCount;
    uint8_t _currentOperationIndex = 0;
    uint8_t _currentProgressBarIndex = 0;
    static constexpr uint8_t BarLength = 16;
    uint8_t _backlightPin;

    int16_t _brightness = 255;
    int16_t _brightnessStep = 5;
    bool _pwmDirection = false;
    static constexpr uint32_t _idlePwmPeriodMs = 50;
    static constexpr uint32_t _idleTextPeriodMs = 1000;

    //MafiaRole _currentEnding = MafiaRole::Citizen;
};
