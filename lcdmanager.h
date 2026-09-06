#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "rolemanager.h"

class LcdManager
{
  public:
    // Конструктор
    LcdManager(LCD_1602_RUS* lcd, const char** loadingMessages, uint8_t loadingMessagesCount, uint8_t backlightPin);
    // Инициализация
    void Begin();
    void ClearDisplay();
    void ResetPwm();
    void UpdateLoadingMessage();
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
    void ResetLoadingMessages();
    uint8_t GetBrightCRT(uint8_t val);

    static constexpr uint8_t BarLength = 16;
    static constexpr uint32_t _idlePwmPeriodMs = 20;
    static constexpr uint32_t _idleTextPeriodMs = 1000;
    static constexpr uint8_t _maxBrightness = 255;
    static constexpr uint8_t _minBrightness = 25;

    LCD_1602_RUS* _lcd;
    const char** _loadingMessages;
    uint8_t _loadingMessagesCount;
    uint8_t _currentLoadingMessageIndex = 0;
    uint8_t _currentProgressBarIndex = 0;
    uint8_t _backlightPin;

    int16_t _brightness = 255;
    int16_t _brightnessStep = 1;
    bool _pwmDirection = false;


};
