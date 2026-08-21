#define _LCD_TYPE 1  // для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include "player.h"
#include "extint.h"

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
LCD_1602_RUS lcd(0x27, 16, 2);
int progress = 0;
unsigned long timer = 0;

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
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ISR_PIN, INPUT_PULLUP);
  ExtInt::ConfigInterrupt();
  ExtInt::EnableInterrupt();
}

struct Operation
{
  const char* msg;
  int updatePeriodMs;
};

static constexpr Operation operations[2] =
{
  { "operation1...", 10 },
  { "operation2...", 300 },
};

void ClearLcd()
{     
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void UpdateLcd()
{
  static uint8_t currentOperationIndex = 0U;
  static uint8_t currentProgressBarIndex = 0U;

  if (millis() - timer >= operations[currentOperationIndex].updatePeriodMs)
  {
    timer = millis();
    if (currentProgressBarIndex == 0U)
    {
      ClearLcd();
      lcd.setCursor(0, 0);
      currentOperationIndex = (currentOperationIndex == 0) ? 1 : 0;
      lcd.print(operations[currentOperationIndex].msg);
    }
  
    lcd.setCursor(currentProgressBarIndex, 1);
    lcd.write(255);
    currentProgressBarIndex = (currentProgressBarIndex < 15) ? currentProgressBarIndex + 1 : 0;
  }
}

void loop()
{
  ProcessTransitions();
  if (state == State::Music)
  {
    player.Loop();
    UpdateLcd();

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
