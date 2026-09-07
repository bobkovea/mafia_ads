#include "extint.h"
#include "lcdmanager.h"
#include "statemachine.h"
#include "nvmanager.h"
#include "rolemanager.h"
#include "config.h"
#include "musicplayer.h"

StateMachine stateMachine(transitions, TransitionsCount, State::Card);

LCD_1602_RUS lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);
LcdManager lcdManager(&lcd, loadingMessages, LoadingMessagesCount, BacklightPin);
RoleManager roleManager;
MusicPlayer<BuzzerPin> musicPlayer;

// ============ Действия ============
void StartLoading()
{
  musicPlayer.PlayBeep();
  
  do
  {
    musicPlayer.Loop();
  } while (!musicPlayer.IsFinished());

  NvManager::IncrementAttempts();

  lcdManager.ResetPwm();
  lcdManager.ClearDisplay();
  lcdManager.UpdateLoadingMessage();

  roleManager.GenerateRole();
}

void StartEnding()
{
  const MafiaRole role = roleManager.GetRole();
  musicPlayer.PlayRole(role);
  lcdManager.PrintCityFallingAsleep();
  while (!lcdManager.SmoothBacklightOff());
  lcdManager.ClearDisplay();
  delay(2000);
  lcdManager.SetEnding(role);
  while (!lcdManager.SmoothBacklightOn());
}

void StartCard()
{
  lcdManager.ClearDisplay();
  ExtInt::EnableInterrupt();
}

void setup()
{
  randomSeed(analogRead(0));
  pinMode(BacklightPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(IsrPin, INPUT_PULLUP);
  lcdManager.Begin();
  ExtInt::ConfigInterrupt();
  ExtInt::EnableInterrupt();
  NvManager::Initialize();
}

void loop()
{
  const State currentState = stateMachine.Process();

  switch (currentState)
  {
    case State::Card:
      lcdManager.UpdateIdle();
      break;

    case State::Loading:
      if (lcdManager.UpdateLoading())
      {
        stateMachine.TriggerEvent(Event::LoadingFinished);
      }
      break;
    
    case State::Ending:
      //lcdManager.UpdateEnding();
      musicPlayer.Loop();
      if (musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::EndingFinished);
      }
      break;

    default:
      break;
  }
}
