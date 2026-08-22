#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>

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
    LcdManager(LCD_1602_RUS* lcd, const Operation* operations, uint8_t count, uint8_t barLength = 16)
    {
      _lcd = lcd;
      _operations = operations;
      _operationsCount = count;
      _barLength = barLength;
      _currentOperationIndex = 0;
      _currentProgressBarIndex = 0;
      _needUpdateOperation = true;
      _timer = 0;
    }

    // Инициализация
    void Begin()
    {
      _lcd->init();
      _lcd->backlight();
    }

    // Обновление (вызывать в loop)
    void Update()
    {
      if (millis() - _timer >= _operations[_currentOperationIndex].updatePeriodMs)
      {
        _timer = millis();

        // Вывод операции при необходимости
        if (_needUpdateOperation)
        {
           //Clear();
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
    uint32_t _timer;
    uint8_t _barLength;
};
