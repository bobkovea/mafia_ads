#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "player.h"
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
Player player(BUZZER_PIN);

static constexpr Operation operations[] =
{
  { "Поиск в БД...", 100 },
  { "operation2...", 200 },
};

// Автоматически вычисляем размер массива
static constexpr uint8_t OperationsCount = sizeof(operations) / sizeof(operations[0]);
static_assert(OperationsCount > 0, "Operations array cannot be empty");

LCD_1602_RUS lcd(0x27, 16, 2);
LcdManager lcdManager(&lcd, operations, OperationsCount);

// ============ Действия ============
void StartMusic()
{
  player.Play();
}

void StartCard()
{
  ExtInt::EnableInterrupt();
}


void setup()
{
  lcdManager.Begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ISR_PIN, INPUT_PULLUP);
  ExtInt::ConfigInterrupt();
  ExtInt::EnableInterrupt();
}

void loop()
{
  stateMachine.Process();
  const State currentState = stateMachine.GetCurrentState();
  if (currentState == State::Music)
  {
    player.Loop();
    lcdManager.Update();

    if (!player.IsActive())
    {
      player.ChangeMelody();
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
