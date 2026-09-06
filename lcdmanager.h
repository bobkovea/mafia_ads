#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "rolemanager.h"

struct Operation
{
  const char* msg;
};

class LcdManager
{
  public:
    // Конструктор
    LcdManager(LCD_1602_RUS* lcd, const Operation* operations, uint8_t count, uint8_t backlightPin);
    // Инициализация
    void Begin();
    void ClearDisplay();
    void ResetPwm();
    void UpdateOperation();
    void PrintCityFallingAsleep();
    bool SmoothBacklightOff();
    bool SmoothBacklightOn();

    void UpdateIdle();
    bool UpdateLoading();
    void SetEnding(const MafiaRole role);

  private:

    bool IsLoadingFinished();
    bool IsProgressBarFull();
    void UpdateProgressBar();
    void ResetProgressBar();
    void ResetOperations();
    uint8_t GetBrightCRT(uint8_t val);

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
