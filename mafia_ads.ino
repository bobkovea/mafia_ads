#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include "screens/typingscreen.h"
#include "screens/arrowscreen.h"
#include "screenhandler.h"
#include "lcd1602rus_hw.h"
#include "backlight.h"
#include "backlighttransition.h"
#include "shifttransition.h"
#include "vbatmanager.h"

#include "statemachine.h"
#include "rolemanager.h"
#include "config.h"
#include "musicplayer.h"
#include "cardhandler.h"
#include "cardstorage.h"

//SoftwareSerial rfid(CardReaderDataPin, CardReaderDataPin);
//RoleManager roleManager;
//MusicPlayer<BuzzerPin> musicPlayer;
//CardHandler cardHandler(rfid);
//CardStorage cardStorage;

VbatManager<VbatPin, VccMv> vbatManager;
Lcd1602rus_hw lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);

ShiftTransition shiftTransition(lcd);
Backlight<BacklightPin> backlight;
BacklightTransition backlightTransition(backlight);

static constexpr uint32_t IdleScreen0TypingPeriodMs = 120U;
static constexpr uint32_t IdleScreen1TypingPeriodMs = 120U;
static constexpr uint32_t ArrowScreenTypingPeriodMs = 60U;

TypingScreen idleScreen0(lcd, "Клуб \"Утопия\"   представляет...", IdleScreen0TypingPeriodMs);
TypingScreen idleScreen1(lcd, " Интерактивная   игра \"Мафия\"", IdleScreen1TypingPeriodMs);
ArrowScreen arrowScreen(lcd, "Приложи пропуск ", "Узнай свою роль!", ArrowScreenTypingPeriodMs);

static constexpr uint32_t IdleScreen0DurationMs = 7000U;
static constexpr uint32_t IdleScreen1DurationMs = 7000U;
static constexpr uint32_t ArrowScreenDurationMs = 15000U;

ScreenItem idleScreenItems[] =
{
  { &idleScreen0, IdleScreen0DurationMs },
  { &idleScreen1, IdleScreen1DurationMs },
  { &arrowScreen, ArrowScreenDurationMs },
};
static constexpr uint8_t IdleScreenCount = sizeof(idleScreenItems) / sizeof(idleScreenItems[0]);
static constexpr uint32_t IdleScreensetDurationMs = 30000U;

Screenset idleScreenset =
{
  idleScreenItems,
  IdleScreenCount,
  IdleScreensetDurationMs,
  &backlightTransition
};

static constexpr uint32_t EndingScreenTypingPeriodMs = 50U;

TypingScreen endingScreen0(lcd, "Запись на игры  в группе ВК:", EndingScreenTypingPeriodMs);
TypingScreen endingScreen1(lcd, "  vk.com/         mafiautopia", EndingScreenTypingPeriodMs);
TypingScreen endingScreen2(lcd, "Кафе \"ЧЧ Лаунж\" пр.Ленина, 71а", EndingScreenTypingPeriodMs);

static constexpr uint32_t EndingScreen0DurationMs = 4000U;
static constexpr uint32_t EndingScreen1DurationMs = 8000U;
static constexpr uint32_t EndingScreen2DurationMs = 5000U;

ScreenItem endingScreenItems[] =
{
  { &endingScreen0, EndingScreen0DurationMs },
  { &endingScreen1, EndingScreen1DurationMs },
  { &endingScreen2, EndingScreen2DurationMs },
};

static constexpr uint8_t EndingScreenCount = sizeof(endingScreenItems) / sizeof(endingScreenItems[0]);

static constexpr uint32_t EndingScreensetDurationMs = 30000U;

Screenset endingScreenset =
{
  endingScreenItems,
  EndingScreenCount,
  EndingScreensetDurationMs,
  &shiftTransition
};

ScreenHandler screenHandler(&idleScreenset);
StateMachine stateMachine(transitions, TransitionsCount, State::Idle);

//void StartEnding()
//{
//  musicPlayer.PlayBeep();
//
//  do
//  {
//    musicPlayer.Loop();
//  } while (!musicPlayer.IsFinished());
//
//  rfid.end();
//
//  pwmManager.ResetPwm();
//  lcdManager.ClearDisplay();
//  roleManager.GenerateRole();
//
//  const MafiaRole role = roleManager.GetRole();
//  musicPlayer.PlayRole(role);
//  lcdManager.PrintCityFallingAsleep();
//  while (!pwmManager.SmoothBacklightOff());
//  lcdManager.ClearDisplay();
//  delay(2000);
//  lcdManager.SetEnding(role);
//  while (!pwmManager.SmoothBacklightOn());
//}
//
//void StartCard()
//{
//  lcdManager.ClearDisplay();
//  lcdManager.PrintIdleMessage();
//  cardHandler.Prepare();
//  rfid.begin(9600);
//}

void setup()
{
  randomSeed(analogRead(A0));
  //  pinMode(BuzzerPin, OUTPUT);
  //  pinMode(CardReaderPowerPin, OUTPUT);
  //  digitalWrite(CardReaderPowerPin, HIGH);
  pinMode(BacklightPin, OUTPUT);
  digitalWrite(BacklightPin, HIGH);
  lcd.begin();
  screenHandler.Begin();
}

void loop()
{
  const State currentState = stateMachine.Process();
  HandleState(currentState);
}

void HandleState(const State state)
{
  const uint32_t now = millis();

  switch (state)
  {
    case State::Idle:
      screenHandler.Handle(now);
      if (screenHandler.IsGroupFinished())
      {
        stateMachine.TriggerEvent(Event::GoodCardDetected);
      }
      break;


    case State::GoodCardIndication:
      backlight.TickDiscrete(now);
      if (backlight.IsDiscreteEnded())
      {
        stateMachine.TriggerEvent(Event::CardIndicationFinished);
      }
      break;

    case State::Ending:
      screenHandler.Handle(now);
      if (screenHandler.IsGroupFinished())
      {
        stateMachine.TriggerEvent(Event::EndingFinished);
      }
      break;
  }
}

void StartGoodCardIndication()
{
  backlight.ResetDiscrete();
  backlight.SetTotalDicsreteCycles(4U);
}

void StartBadCardIndication()
{

}
void StartSlotMachine()
{

}
void StartEnding()
{
  screenHandler.SetScreenset(&endingScreenset);
}
void StartIdle()
{
  screenHandler.SetScreenset(&idleScreenset);
}

//
//void HandleState(const State state)
//{
//  switch (state)
//  {
//    case State::Card:
//      pwmManager.Breath();
//      lcdManager.UpdateIdleAnimation();
//      if (cardHandler.Handle())
//      {
//        uint8_t id[5];
//        cardHandler.GetLastId(id);
//        const uint32_t timeLeftMs = cardStorage.AddId(id);
//        if (timeLeftMs == 0U)
//        {
//          stateMachine.TriggerEvent(Event::CardDetected);
//        }
//        else
//        {
//          rfid.end();
//
//          musicPlayer.PlayOops();
//          do
//          {
//            musicPlayer.Loop();
//          } while (!musicPlayer.IsFinished());
//
//          pwmManager.ResetPwm();
//          lcdManager.ClearDisplay();
//          for (uint8_t i = 0; i < 5; ++i)
//          {
//            // переход через ноль
//            const int32_t timeLeftDisplayed = (int32_t)timeLeftMs - ((int32_t)1000 * (int32_t)i);
//            lcdManager.PrintTimeLeft(timeLeftDisplayed < 0 ? 0U : (uint32_t)timeLeftDisplayed);
//            delay(1000);
//          }
//
//          lcdManager.ClearDisplay();
//          lcdManager.PrintIdleMessage();
//          cardHandler.Prepare();
//          rfid.begin(9600);
//        }
//      }
//      break;
//
//    case State::Ending:
//
//      musicPlayer.Loop();
//      if (musicPlayer.IsFinished())
//      {
//        stateMachine.TriggerEvent(Event::EndingFinished);
//      }
//      break;
//
//    default:
//      break;
//  }
//}
