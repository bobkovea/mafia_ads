#include "extint.h"
#include "lcdmanager.h"
#include "statemachine.h"
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"
#include "melodies/doorbeep.h"
#include "nvmanager.h"
#include "BuzzerMelody.h"
#include "rolemanager.h"
#include "config.h"

// ============ Глобальные переменные ============
StateMachine stateMachine(transitions, TransitionsCount, State::Card);

BuzzerMelody beep = BuzzerMelody(BuzzerPin, DoorBeep::melodyLength, DoorBeep::melody);

BuzzerMelody melodies[]
{
  BuzzerMelody(BuzzerPin, Aha::melodyLength, Aha::melody),
  BuzzerMelody(BuzzerPin, Pirates::melodyLength, Pirates::melody),
  BuzzerMelody(BuzzerPin, PinkPanther::melodyLength, PinkPanther::melody),
  BuzzerMelody(BuzzerPin, Godfather::melodyLength, Godfather::melody),
};

LCD_1602_RUS lcd(0x27, 16, 2);
LcdManager lcdManager(&lcd, operations, OperationsCount, BacklightPin);
RoleManager roleManager;

// ============ Действия ============
void StartLoading()
{
  beep.play();

  do
  {
    beep.loop();
  } while (beep.getState() != BuzzerMelody::IDLE);

  NvManager::IncrementAttempts();

  lcdManager.ResetPwm();
  lcdManager.ClearDisplay();
  lcdManager.UpdateOperation();

  roleManager.GenerateRole();
}

void StartEnding()
{
  const uint8_t roleNumber = (uint8_t)roleManager.GetRole();
  melodies[roleNumber].play();

  lcdManager.PrintCityFallingAsleep();
  while (!lcdManager.SmoothBacklightOff());
  lcdManager.ClearDisplay();
  delay(2000);
  lcdManager.SetEnding((MafiaRole)roleNumber);
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
      const uint8_t roleNumber = (uint8_t)roleManager.GetRole();
      melodies[roleNumber].loop();
      if (melodies[roleNumber].getState() == BuzzerMelody::IDLE)
      {
        stateMachine.TriggerEvent(Event::EndingFinished);
      }

      break;

    default:
      break;
  }
}
