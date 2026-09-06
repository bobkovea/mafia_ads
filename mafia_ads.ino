#include "musicplayer.h"
#include "extint.h"
#include "lcdmanager.h"
#include "statemachine.h"
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"
#include "melodies/doorbeep.h"
#include "nvmanager.h"

#define ISR_PIN 2
#define BUZZER_PIN 3
#define BACKLIGHT_PIN 9

// ============ Прототипы действий ============
void StartLoading();
void StartEnding();
void StartCard();

// ============ Таблица переходов ============
const Transition transitions[] = {
  { State::Card, (uint8_t)Event::CardDetected, State::Loading, StartLoading },
  { State::Loading, (uint8_t)Event::LoadingFinished, State::Ending, StartEnding },
  { State::Ending, (uint8_t)Event::EndingFinished, State::Card, StartCard },
};

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");

// ============ Глобальные переменные ============
StateMachine stateMachine(transitions, TransitionsCount, State::Card);

static constexpr Operation operations[] =
{ //"XXXXXXXXXXXXXXXX"
  { "Поиск в базе... " },
  { "Расчет модели..." },
  { "Подбор роли...  " }
};

// Автоматически вычисляем размер массива
static constexpr uint8_t OperationsCount = sizeof(operations) / sizeof(operations[0]);
static_assert(OperationsCount > 0, "Operations array cannot be empty");

BuzzerMelody beep = BuzzerMelody(BUZZER_PIN, DoorBeep::melodyLength, DoorBeep::melody);

BuzzerMelody melodies[]
{
  BuzzerMelody(BUZZER_PIN, Aha::melodyLength, Aha::melody),
  BuzzerMelody(BUZZER_PIN, Pirates::melodyLength, Pirates::melody),
  BuzzerMelody(BUZZER_PIN, PinkPanther::melodyLength, PinkPanther::melody),
  BuzzerMelody(BUZZER_PIN, Godfather::melodyLength, Godfather::melody),
};

// Автоматически вычисляем размер массива
static constexpr uint8_t MelodiesCount = sizeof(melodies) / sizeof(melodies[0]);
static_assert(MelodiesCount > 0, "Melodies array cannot be empty");

MusicPlayer musicPlayer(melodies, MelodiesCount, 0);
LCD_1602_RUS lcd(0x27, 16, 2);
LcdManager lcdManager(&lcd, operations, OperationsCount);
MafiaRole role;

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

  role = GetRole();
}

void StartEnding()
{
   melodies[(uint8_t)role].play();
   
   lcdManager.PrintCityFallingAsleep();
   while(!lcdManager.SmoothBacklightOff());
   lcdManager.ClearDisplay();
   delay(2000);
   lcdManager.SetEnding(role);
   while(!lcdManager.SmoothBacklightOn());
}

void StartCard()
{
  lcdManager.ClearDisplay();
  ExtInt::EnableInterrupt();
}

MafiaRole GetRole()
{
  return (MafiaRole)random(4);
}

void setup()
{
  randomSeed(analogRead(0));
  pinMode(BACKLIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ISR_PIN, INPUT_PULLUP);
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
      melodies[(uint8_t)role].loop();
      if (melodies[(uint8_t)role].getState() == BuzzerMelody::IDLE)
      {
        stateMachine.TriggerEvent(Event::EndingFinished);
      }

      break;

    default:
      break;
  }
}

// ============ Прерывание карты ============
ISR(INT0_vect)
{
  ExtInt::DisableInterrupt();
  stateMachine.TriggerEvent(Event::CardDetected);
}
