#include <EEPROM.h>
#include "musicplayer.h"
#include "extint.h"
#include "lcdmanager.h"
#include "statemachine.h"
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"

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

// ============ Действия ============
void StartMusic()
{
  uint32_t cardsDetected;
  EEPROM.get(0, cardsDetected);
  cardsDetected = (cardsDetected == UINT32_MAX) ? 0 : cardsDetected + 1;
  EEPROM.put(0, cardsDetected);

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

  uint32_t tmp;
  EEPROM.get(0, tmp);
  if (tmp == UINT32_MAX)
  {
    EEPROM.put(0, tmp);
  }
}

void loop()
{
  const State currentState = stateMachine.Process();

  switch (currentState)
  {
    case State::Card:
      //      uint32_t var;
      //      EEPROM.get(0, var);
      //      lcd.setCursor(0, 0);
      //      lcd.print(var, 10);
      //      delay(100);
      break;

    case State::Music:
      musicPlayer.Loop();
      lcdManager.Update();

      if (!musicPlayer.IsActive())
      {
        musicPlayer.ChangeMelody();
        stateMachine.TriggerEvent(Event::MusicFinished);
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
