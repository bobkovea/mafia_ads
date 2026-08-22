#include "musicplayer.h"
#include "extint.h"
#include "lcdmanager.h"
#include "statemachine.h"

#define ISR_PIN 2
#define BUZZER_PIN 9

// ============ Прототипы действий ============
void StartMusic();
void StartCard();

// ============ Таблица переходов ============
const Transition transitions[] = {
  {State::Card, (uint8_t)Event::CardDetected, State::Music, StartMusic},
  {State::Music, (uint8_t)Event::MusicFinished, State::Card, StartCard},
};

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");

// ============ Глобальные переменные ============
StateMachine stateMachine(transitions, TransitionsCount, State::Card);

static constexpr Operation operations[] =
{ //"XXXXXXXXXXXXXXXX"
  { "ПOИCK В БАЗЕ... ", 100},
  { "СЪЕМ ПАТТЕРНА...", 100},
  { "РАСЧЕТ МОДЕЛИ...", 100},
  { "ПОДБОР РОЛИ...  ", 100},
};

// Автоматически вычисляем размер массива
static constexpr uint8_t OperationsCount = sizeof(operations) / sizeof(operations[0]);
static_assert(OperationsCount > 0, "Operations array cannot be empty");

BuzzerMelody melodies[]
{
 { BUZZER_PIN, Harry::melodyLength, Harry::melody },
 { BUZZER_PIN, Mario::melodyLength, Mario::melody },
};

// Автоматически вычисляем размер массива
static constexpr uint8_t MelodiesCount = sizeof(melodies) / sizeof(melodies[0]);
static_assert(MelodiesCount > 0, "Melodies array cannot be empty");

MusicPlayer musicPlayer(melodies, MelodiesCount, 0);
LCD_1602_RUS lcd(0x27, 16, 2);
LcdManager lcdManager(&lcd, operations, OperationsCount);

// ============ Действия ============
void StartMusic()
{
  musicPlayer.Play();
}

void StartCard()
{
  ExtInt::EnableInterrupt();
}



void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ISR_PIN, INPUT_PULLUP);
  lcdManager.Begin();
  ExtInt::ConfigInterrupt();
  ExtInt::EnableInterrupt();
}

void loop()
{
  stateMachine.Process();
  const State currentState = stateMachine.GetCurrentState();
  if (currentState == State::Music)
  {
    musicPlayer.Loop();
    lcdManager.Update();

    if (!musicPlayer.IsActive())
    {
      musicPlayer.ChangeMelody();
      stateMachine.TriggerEvent(Event::MusicFinished);
    }
  }
}

// ============ Прерывание карты ============
ISR(INT0_vect)
{
  stateMachine.TriggerEvent(Event::CardDetected);
  ExtInt::DisableInterrupt();
}
