#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include "screens/typingscreen.h"
#include "screens/arrowscreen.h"
#include "screens/batteryscreen.h"
#include "screenhandler.h"
#include "lcd1602rus_hw.h"
#include "backlight.h"
#include "backlighttransition.h"
#include "shifttransition.h"
#include "batterymanager.h"

#include "statemachine.h"
#include "rolemanager.h"
#include "config.h"
#include "musicplayer.h"
#include "cardhandler.h"
#include "cardstorage.h"

//SoftwareSerial rfid(CardReaderDataPin, CardReaderDataPin);
//RoleManager roleManager;
MusicPlayer<BuzzerPin> musicPlayer;
//CardHandler cardHandler(rfid);
//CardStorage cardStorage;

BatteryManager<VbatPin, VccMv> batteryManager;
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
static constexpr uint32_t IdleScreensetDurationMs = 20000U;

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

static constexpr uint32_t EndingScreensetDurationMs = 20000U;

Screenset endingScreenset =
{
  endingScreenItems,
  EndingScreenCount,
  EndingScreensetDurationMs,
  &shiftTransition
};

static constexpr uint32_t BatteryScreenTypingPeriodMs = 120U;
BatteryScreen batteryScreen(lcd, batteryManager);

static constexpr uint32_t BatteryScreenDurationMs = 20000U;

ScreenItem batteryScreenItems[] =
{
  { &batteryScreen, BatteryScreenDurationMs }
};

static constexpr uint8_t BatteryScreenCount = sizeof(batteryScreenItems) / sizeof(batteryScreenItems[0]);
static constexpr uint32_t BatteryScreensetDurationMs = 5000U;

Screenset batteryScreenset =
{
  batteryScreenItems,
  BatteryScreenCount,
  BatteryScreensetDurationMs,
  &backlightTransition // dummy
};

ScreenHandler screenHandler(&idleScreenset);
StateMachine stateMachine(transitions, TransitionsCount, State::Idle);

void setup()
{
  randomSeed(analogRead(A0));
  pinMode(BuzzerPin, OUTPUT);
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
  const uint32_t now = millis(); // todo какой-то баг с порядком millis
  HandleState(currentState, now);
  batteryManager.Tick(now);
}

void HandleState(const State state, const uint32_t now)
{
  switch (state)
  {
    case State::Idle:
      screenHandler.Handle(now);
      if (screenHandler.IsGroupFinished())
      {
        stateMachine.TriggerEvent(Event::GoodCardDetected);
        // stateMachine.TriggerEvent(Event::BatteryIndicationRequested);
      }
      break;

    case State::BatteryIndication:
      screenHandler.Handle(now);
      if (screenHandler.IsGroupFinished())
      {
        stateMachine.TriggerEvent(Event::BatteryIndicationFinished);
      }
      break;

    case State::GoodCardIndication:
      musicPlayer.Loop();
      backlight.TickDiscrete(now);
      if (backlight.IsDiscreteEnded() && musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::CardIndicationFinished);
      }
      break;

    case State::Ending:
      screenHandler.Handle(now);
      musicPlayer.Loop();
      if (screenHandler.IsGroupFinished() && musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::EndingFinished);
      }
      break;
  }
}

void StartGoodCardIndication()
{
  musicPlayer.PlayBeep();
  backlight.ResetDiscrete();
  backlight.SetTotalDicsreteCycles(5U);
}

void StartBadCardIndication()
{

}

void StartSlotMachine()
{

}

void StartEnding()
{
  musicPlayer.PlayRole(MafiaRole::Don);
  screenHandler.SetScreenset(&endingScreenset);
}
void StartIdle()
{
  screenHandler.SetScreenset(&idleScreenset);
}

void StartBatteryIndication()
{
  screenHandler.SetScreenset(&batteryScreenset);
}


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
