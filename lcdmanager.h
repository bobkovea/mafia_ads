#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include <GTimer.h>

// Структура операции
struct Operation
{
  const char* msg;
  uint32_t updatePeriodMs;
};

// Класс для управления LCD с прогресс-баром
class LcdManager
{
  public:
    // Конструктор
    LcdManager(LCD_1602_RUS* lcd, const Operation* operations, uint8_t count, uint8_t barLength = 16, uint8_t backlightPin = 9)
    {
      _lcd = lcd;
      _operations = operations;
      _operationsCount = count;
      _barLength = barLength;
      _currentOperationIndex = 0;
      _currentProgressBarIndex = 0;
      _needUpdateOperation = true;
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

    void UpdateLoading()
    {
      EVERY_MS(_operations[_currentOperationIndex].updatePeriodMs)
      {
        // Вывод операции при необходимости
        if (_needUpdateOperation)
        {
          _lcd->setCursor(0, 1);
          _lcd->print("                ");
          _lcd->setCursor(0, 0);
          _lcd->print(_operations[_currentOperationIndex].msg);
          _needUpdateOperation = false;
        }

        // Вывод прогресс-бара
        _lcd->setCursor(_currentProgressBarIndex, 1);
        _lcd->write(255);
        _currentProgressBarIndex++;

        // Переключение при заполнении
        if (_currentProgressBarIndex >= _barLength)
        {
          _currentProgressBarIndex = 0;
          _currentOperationIndex = (_currentOperationIndex + 1) % _operationsCount;
          _needUpdateOperation = true;
        }
      }
    }

  private:
    LCD_1602_RUS* _lcd;
    const Operation* _operations;
    uint8_t _operationsCount;
    uint8_t _currentOperationIndex;
    uint8_t _currentProgressBarIndex;
    bool _needUpdateOperation;
    uint8_t _barLength;
    uint8_t _backlightPin;


    int16_t _brightness = 255;
    int16_t _brightnessStep = 5;
    bool _pwmDirection = false;
    static constexpr uint32_t _idlePwmPeriodMs = 50;
    static constexpr uint32_t _idleTextPeriodMs = 1000;
};
