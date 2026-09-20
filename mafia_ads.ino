#include <SoftwareSerial.h>
#include "lcdmanager.h"
#include "statemachine.h"
#include "rolemanager.h"
#include "config.h"
#include "musicplayer.h"
#include "pwmmanager.h"
#include "cardhandler.h"
#include "cardstorage.h"

SoftwareSerial rfid(CardReaderDataPin, CardReaderDataPin);
StateMachine stateMachine(transitions, TransitionsCount, State::Card);
LCD_1602_RUS lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);
LcdManager lcdManager(&lcd, loadingMessages, LoadingMessagesCount);
RoleManager roleManager;
MusicPlayer<BuzzerPin> musicPlayer;
PwmManager<BacklightPin> pwmManager;
CardHandler cardHandler(rfid);
CardStorage cardStorage;

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
  cardHandler.Prepare();
  lcdManager.ClearDisplay();
  lcdManager.PrintIdleMessage();
}

void setup()
{
  randomSeed(analogRead(0));
  pinMode(BacklightPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(CardReaderPowerPin, OUTPUT);
  digitalWrite(CardReaderPowerPin, HIGH);
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
      //lcdManager.UpdateIdleAnimation();
      if (cardHandler.Handle())
      {
        uint8_t id[5];
        cardHandler.GetLastId(id);
        const uint32_t timeLeftMs = cardStorage.AddId(id);
        if (timeLeftMs == 0U)
        {
          stateMachine.TriggerEvent(Event::CardDetected);
        }
        else
        {
          lcdManager.PrintTimeLeft(timeLeftMs);
          musicPlayer.PlayOops();
          do
          {
            musicPlayer.Loop();
          } while (!musicPlayer.IsFinished());
        }
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
