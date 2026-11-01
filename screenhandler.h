#pragma once

#include "contracts/itransition.h"
#include "contracts/iscreen.h"

struct ScreenItem
{
  IScreen* screen;
  uint32_t durationMs;
};

struct Screenset
{
  ScreenItem* screenItems;
  uint8_t screensCount;
  uint32_t totalTimeMs;
  ITransition* transition;
};

enum class ScreenStatus : uint8_t
{
  InProgress,
  ScreenTransitionStarted,
  ScreenTransitionHalf,
  GroupFinished
};

class ScreenHandler
{
  public:

    ScreenHandler(Screenset* screenset) :
      _screenset(screenset)
    {
    };

    void Begin()
    {
      _currentScreen = 0;
      GetCurrentScreen()->OnEnter();
      _status = ScreenStatus::InProgress;
      const uint32_t now = millis();
      _lastSwitch = _lastEnter = now;
      GetTransition()->Start(now);
    }

    void SetScreenset(Screenset* screenset)
    {
      _screenset = screenset;
      Begin();
    }

    void Handle(const uint32_t now)
    {
      GetCurrentScreen()->Tick(now);

      switch (_status)
      {
        case ScreenStatus::InProgress:
          if ((now - _lastEnter >= GetScreensetTotalTimeMs()) && (GetScreensetTotalTimeMs() != 0U))
          {
            _status = ScreenStatus::GroupFinished;
          }
          else if (now - _lastSwitch >= GetScreenDurationMs())
          {
            GetTransition()->Start(now);
            _status = ScreenStatus::ScreenTransitionStarted;
          }
          break;

        case ScreenStatus::ScreenTransitionStarted:
          GetTransition()->Tick(now, TransitionPhase::FadeOut);
          if (GetTransition()->IsHidden())
          {
            _currentScreen = (_currentScreen + 1) % GetScreensCount();
            GetCurrentScreen()->OnEnter();
            _status = ScreenStatus::ScreenTransitionHalf;
          }
          break;

        case ScreenStatus::ScreenTransitionHalf:
          GetTransition()->Tick(now, TransitionPhase::FadeIn);
          if (GetTransition()->IsVisible())
          {
            _lastSwitch = now;
            _status = ScreenStatus::InProgress;
          }
          break;

        case ScreenStatus::GroupFinished:
          break;
      }
    }

    bool IsGroupFinished()
    {
      return _status == ScreenStatus::GroupFinished;
    }

  private:

    inline IScreen* GetCurrentScreen()
    {
      return _screenset->screenItems[_currentScreen].screen;
    };

    inline uint32_t GetScreenDurationMs()
    {
      return _screenset->screenItems[_currentScreen].durationMs;
    };

    inline uint8_t GetScreensCount()
    {
      return _screenset->screensCount;
    };

    inline uint32_t GetScreensetTotalTimeMs()
    {
      return _screenset->totalTimeMs;
    };

    inline ITransition* GetTransition()
    {
      return _screenset->transition;
    }

    Screenset* _screenset;
    uint8_t _currentScreen = 0U;
    uint32_t _lastSwitch = 0U;
    uint32_t _lastEnter = 0U;
    ScreenStatus _status = ScreenStatus::InProgress;
};
