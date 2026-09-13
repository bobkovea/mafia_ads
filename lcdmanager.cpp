#include "lcdmanager.h"
#include <GTimer.h>

LcdManager::LcdManager(LCD_1602_RUS* lcd, const char** loadingMessages, uint8_t loadingMessagesCount) :
  _lcd(lcd),
  _loadingMessages(loadingMessages),
  _loadingMessagesCount(loadingMessagesCount)
{
}

void LcdManager::Begin()
{
  _lcd->init();
  _lcd->backlight();
}

void LcdManager::ClearDisplay()
{
  _lcd->setCursor(0, 0);
  _lcd->print("                ");
  _lcd->setCursor(0, 1);
  _lcd->print("                ");
}

void LcdManager::PrintCityFallingAsleep()
{
  _lcd->setCursor(0, 0);
  _lcd->print({"   Город        "});
  _lcd->setCursor(0, 1);
  _lcd->print({"   засыпает...  "});
}

void LcdManager::UpdateLoadingMessage()
{
  _lcd->setCursor(0, 0);
  _lcd->print(_loadingMessages[_currentLoadingMessageIndex]);
  ++_currentLoadingMessageIndex;
}

void LcdManager::PrintIdleMessage()
{
  _lcd->setCursor(0, 0);
  _lcd->print("Жду пропуск");
}

void LcdManager::UpdateIdleAnimation()
{
  EVERY_MS(_idleTextPeriodMs)
  {
    static uint8_t pos = 11;
        
    if (pos == 14)
    {
      pos = 11;
      _lcd->setCursor(pos, 0);
      _lcd->print("   ");
    }

    _lcd->setCursor(pos++, 0);
    _lcd->print(".");
  }
}

bool LcdManager::IsLoadingFinished()
{
  return (_currentLoadingMessageIndex == _loadingMessagesCount) && (_currentProgressBarIndex == BarLength);
}

bool LcdManager::IsProgressBarFull()
{
  return _currentProgressBarIndex == BarLength;
}

void LcdManager::UpdateProgressBar()
{
  _lcd->setCursor(_currentProgressBarIndex, 1);
  _lcd->write(255);
  ++_currentProgressBarIndex;
}

void LcdManager::ResetProgressBar()
{
  _currentProgressBarIndex = 0;
  _lcd->setCursor(0, 1);
  _lcd->print("                ");
}

void LcdManager::ResetLoadingMessages()
{
  _currentLoadingMessageIndex = 0;
  _lcd->setCursor(0, 0);
  _lcd->print("                ");
}

bool LcdManager::UpdateLoading()
{
  static uint32_t period = 100;
  bool loadingIsFinished = false;

  EVERY_MS(period)
  {
    // period = (uint32_t)random(50, 100); //TBD

    loadingIsFinished = IsLoadingFinished();

    if (loadingIsFinished)
    {
      ResetLoadingMessages();
      ResetProgressBar();
    }
    else
    {
      if (IsProgressBarFull())
      {
        ResetProgressBar();
        UpdateLoadingMessage();
      }
      else
      {
        UpdateProgressBar();
      }
    }
  }
  return loadingIsFinished;
}

void LcdManager::SetEnding(const MafiaRole role)
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
