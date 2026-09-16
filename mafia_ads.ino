#include "lcdmanager.h"
#include "statemachine.h"
#include "rolemanager.h"
#include "config.h"
#include "musicplayer.h"
#include "pwmmanager.h"
#include "cardhandler.h"
#include <SoftwareSerial.h>

SoftwareSerial rfid(2, 2);
StateMachine stateMachine(transitions, TransitionsCount, State::Card);
LCD_1602_RUS lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);
LcdManager lcdManager(&lcd, loadingMessages, LoadingMessagesCount);
RoleManager roleManager;
MusicPlayer<BuzzerPin> musicPlayer;
PwmManager<BacklightPin> pwmManager;
CardHandler cardHandler(rfid);

void StartLoading()
{
  musicPlayer.PlayBeep();

  do
  {
    musicPlayer.Loop();
  } while (!musicPlayer.IsFinished());

  pwmManager.ResetPwm();
  lcdManager.ResetIdleAnimation();
  lcdManager.UpdateLoadingMessage();

  roleManager.GenerateRole();
}

void StartEnding()
{
  const MafiaRole role = roleManager.GetRole();
  musicPlayer.PlayRole(role);
  lcdManager.PrintCityFallingAsleep();
  while (!pwmManager.SmoothBacklightOff());
  lcdManager.ClearDisplay();
  delay(2000);
  lcdManager.SetEnding(role);
  while (!pwmManager.SmoothBacklightOn());
}

void StartCard()
{
  lcdManager.ClearDisplay();
  lcdManager.PrintIdleMessage();
}

void setup()
{
  randomSeed(analogRead(0));
  pinMode(BacklightPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(CardReaderPin, OUTPUT);
  digitalWrite(CardReaderPin, HIGH);
  lcdManager.Begin();
  lcdManager.PrintIdleMessage();
}

void loop()
{
  const State currentState = stateMachine.Process();

  switch (currentState)
  {
    case State::Card:
      pwmManager.Breath();
      lcdManager.UpdateIdleAnimation();
      if (cardHandler.Handle())
      {
        stateMachine.TriggerEvent(Event::CardDetected);
      }
      break;

    case State::Loading:
      if (lcdManager.UpdateLoading())
      {
        stateMachine.TriggerEvent(Event::LoadingFinished);
      }
      break;

    case State::Ending:

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
