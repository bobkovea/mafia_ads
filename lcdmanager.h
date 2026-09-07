#pragma once

#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "rolemanager.h"

class LcdManager
{
  public:
    LcdManager(LCD_1602_RUS* lcd, const char** loadingMessages, uint8_t loadingMessagesCount);
    void Begin();
    void ClearDisplay();
    void UpdateLoadingMessage();
    void PrintCityFallingAsleep();
    void UpdateAttemptsMessage();
    bool UpdateLoading();
    void SetEnding(const MafiaRole role);

  private:

    bool IsLoadingFinished();
    bool IsProgressBarFull();
    void UpdateProgressBar();
    void ResetProgressBar();
    void ResetLoadingMessages();

    static constexpr uint8_t BarLength = 16;
    static constexpr uint32_t _idleTextPeriodMs = 1000;

    LCD_1602_RUS* _lcd;
    const char** _loadingMessages;
    uint8_t _loadingMessagesCount;
    uint8_t _currentLoadingMessageIndex = 0;
    uint8_t _currentProgressBarIndex = 0;
};
