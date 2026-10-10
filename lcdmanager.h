#pragma once

#include "lcd1602rus_hw.h"
#include "rolemanager.h"

class LcdManager
{
  public:
    LcdManager(Lcd1602rus_hw* lcd, const char** loadingMessages, uint8_t loadingMessagesCount);
    void Begin();
    void ClearDisplay();
    void UpdateLoadingMessage();
    void PrintCityFallingAsleep();
    bool UpdateLoading();
    void SetEnding(const MafiaRole role);
    void PrintIdleMessage();
    void UpdateIdleAnimation();
    void ResetIdleAnimation();
    void PrintTimeLeft(uint32_t timeLeftMs);

  private:

    bool IsLoadingFinished();
    bool IsProgressBarFull();
    void UpdateProgressBar();
    void ResetProgressBar();
    void ResetLoadingMessages();

    static constexpr uint8_t BarLength = 16;
    static constexpr uint32_t _idleFrameTimeMs = 1000;
    uint8_t _idleDotPosition = 11;

    const char** _loadingMessages;
    uint8_t _loadingMessagesCount;
    uint8_t _currentLoadingMessageIndex = 0;
    uint8_t _currentProgressBarIndex = 0;
    Lcd1602rus_hw* _lcd;
};
