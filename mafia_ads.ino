#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "player.h"
#include "extint.h"
#include "lcdmanager.h"

#define ISR_PIN 2
#define BUZZER_PIN 9

// ============ Состояния ============
enum class State : uint8_t
{
  Card,
  Music
};

// ============ События ============
enum class Event : uint8_t
{
  None = 0x00,
  CardDetected = 0x01,
  MusicFinished = 0x02
};


// ============ Таблица переходов ============
struct Transition
{
  State currentState;
  uint8_t event;
  State nextState;
  void (*action)();
};

// ============ Прототипы действий ============
void StartMusic();
void StartCard();

// ============ Таблица переходов ============
const Transition transitions[] = {
  {State::Card, (uint8_t)Event::CardDetected, State::Music, StartMusic},
  {State::Music, (uint8_t)Event::MusicFinished, State::Card, StartCard},
};

// ============ Глобальные переменные ============
volatile State state = State::Card;
volatile uint8_t events = 0;
Player player(BUZZER_PIN);

static constexpr Operation operations[] =
{
  { "operation1...", 100 },
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

// ============ Обработка переходов ============
void ProcessTransitions()
{
  if (events != (uint8_t)Event::None)
  {
    const uint8_t currentEvents = events;
    events = (uint8_t)Event::None;

    for (const auto& trans : transitions)
    {
      const bool stateMatches = trans.currentState == state;
      const bool eventMatches = (trans.event & currentEvents) != 0;
      if (stateMatches && eventMatches)
      {
        trans.action();
        state = trans.nextState;
        break;
      }
    }
  }
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
  ProcessTransitions();
  if (state == State::Music)
  {
    player.Loop();
    lcdManager.Update();

    if (!player.IsActive())
    {
      player.ChangeMelody();
      events |= (uint8_t)Event::MusicFinished;
    }
  }
}

// ============ Прерывание карты ============
ISR(INT0_vect)
{
  events |= (uint8_t)Event::CardDetected;
  ExtInt::DisableInterrupt();
}
