#include "lcdmanager.h"

#include <GTimer.h>
#include "nvmanager.h"

LcdManager::LcdManager(LCD_1602_RUS* lcd, const Operation* operations, uint8_t count, uint8_t backlightPin) :
  _lcd(lcd),
  _operations(operations),
  _operationsCount(count),
  _backlightPin(backlightPin)
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

void LcdManager::ResetPwm()
{
  _pwmDirection = false;
  _brightness = 255;
  analogWrite(_backlightPin, _brightness);
}

void LcdManager::PrintCityFallingAsleep()
{
  _lcd->setCursor(0, 0);
  _lcd->print({"   Город        "});
  _lcd->setCursor(0, 1);
  _lcd->print({"   засыпает...  "});
}

bool LcdManager::SmoothBacklightOff()
{
  EVERY_MS(_idlePwmPeriodMs)
  {
    analogWrite(_backlightPin, GetBrightCRT(_brightness));

    _brightness -= _brightnessStep;

    if (_brightness < _minBrightness)
    {
      _brightness = _minBrightness;
    }
  }
  return _brightness == _minBrightness;
}

bool LcdManager::SmoothBacklightOn()
{
  EVERY_MS(_idlePwmPeriodMs)
  {
    analogWrite(_backlightPin, GetBrightCRT(_brightness));

    _brightness += _brightnessStep;

    if (_brightness > _maxBrightness)
    {
      _brightness = _maxBrightness;
    }
  }
  return _brightness == _maxBrightness;
}

void LcdManager::UpdateIdle()
{
  EVERY_MS(_idlePwmPeriodMs)
  {
    analogWrite(_backlightPin, GetBrightCRT(_brightness));

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

    const uint32_t attempts = NvManager::GetAttempts();
    _lcd->setCursor(0, 1);
    _lcd->print(attempts, 10);
  }
}

void LcdManager::UpdateOperation()
{
  _lcd->setCursor(0, 0);
  _lcd->print(_operations[_currentOperationIndex].msg);
  ++_currentOperationIndex;
}

bool LcdManager::IsLoadingFinished()
{
  return (_currentOperationIndex == _operationsCount) && (_currentProgressBarIndex == BarLength);
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

void LcdManager::ResetOperations()
{
  _currentOperationIndex = 0;
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

uint8_t LcdManager::GetBrightCRT(uint8_t val)
{
  return (uint32_t(val + 1) * (val + 1) * val) >> 16;
}
