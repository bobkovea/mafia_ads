#include <SoftwareSerial.h>
//#include <avr/sleep.h>
#include "screens/typingscreen.h"
#include "screens/arrowscreen.h"
#include "screens/batteryscreen.h"
#include "screens/timeleftscreen.h"
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
#include "card/cardreader.h"
#include "card/cardstorage.h"
#include "card/cardcontroller.h"
#include "utils/flashstring.h"

//RoleManager roleManager;

SoftwareSerial rfid(CardReaderDataPin, CardReaderDataPin);
CardReader cardReader(rfid);
static constexpr int8_t StorageSize = 32; // максимум пропусков в базе
static constexpr uint32_t TimeBetweenMs = 86400000U; // 24 часа
CardStorage<StorageSize, TimeBetweenMs> cardStorage;
CardController cardController(cardReader, cardStorage);

MusicPlayer<BuzzerPin> musicPlayer;
BatteryManager<VbatPin, VccMv> batteryManager;
Lcd1602rus_hw lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);

ShiftTransition shiftTransition(lcd);
Backlight<BacklightPin> backlight;
BacklightTransition backlightTransition(backlight);

static constexpr uint32_t IdleScreen0TypingPeriodMs = 120U;
static constexpr uint32_t IdleScreen1TypingPeriodMs = 120U;
static constexpr uint32_t ArrowScreenTypingPeriodMs = 60U;

static const char IdleText0[] PROGMEM = "Клуб \"Утопия\"   представляет...";
static const char IdleText1[] PROGMEM = " Интерактивная   игра \"Мафия\"";
static const char ArrowText0[] PROGMEM = "Приложи пропуск ";
static const char ArrowText1[] PROGMEM = "Узнай свою роль!";

TypingScreen idleScreen0(lcd, FPSTR(IdleText0), IdleScreen0TypingPeriodMs);
TypingScreen idleScreen1(lcd, FPSTR(IdleText1), IdleScreen1TypingPeriodMs);
ArrowScreen arrowScreen(lcd, FPSTR(ArrowText0), 
  FPSTR(ArrowText1), ArrowScreenTypingPeriodMs);

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
static constexpr uint32_t IdleScreensetDurationMs = UINT32_MAX;

Screenset idleScreenset =
{
  idleScreenItems,
  IdleScreenCount,
  IdleScreensetDurationMs,
  &backlightTransition
};

static constexpr uint32_t EndingScreenTypingPeriodMs = 50U;

static const char EndingText0[] PROGMEM = "Запись на игры  в группе ВК:";
static const char EndingText1[] PROGMEM = "  vk.com/         mafiautopia";
static const char EndingText2[] PROGMEM = "Кафе \"ЧЧ Лаунж\" пр.Ленина, 71а";

TypingScreen endingScreen0(lcd, FPSTR(EndingText0), EndingScreenTypingPeriodMs);
TypingScreen endingScreen1(lcd, FPSTR(EndingText1), EndingScreenTypingPeriodMs);
TypingScreen endingScreen2(lcd, FPSTR(EndingText2), EndingScreenTypingPeriodMs);

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
  pinMode(CardReaderPowerPin, OUTPUT);
  digitalWrite(CardReaderPowerPin, HIGH);
  pinMode(BacklightPin, OUTPUT);
  digitalWrite(BacklightPin, HIGH);
  lcd.begin();
  screenHandler.Begin();
  rfid.begin(9600);
}

void loop()
{
  const State currentState = stateMachine.Process();
  const uint32_t now = millis(); // todo какой-то баг с порядком millis
  HandleState(currentState, now);
//  batteryManager.Tick(now);
}

void HandleState(const State state, const uint32_t now)
{
  switch (state)
  {
    case State::Idle:
        screenHandler.Handle(now);
        switch(cardController.Poll(now))
        {
          case UploadStatus::NewCardAdded:
          case UploadStatus::ExistingCardUpdated:
            stateMachine.TriggerEvent(Event::GoodCardDetected);
          break;

          case UploadStatus::ExistingCardTimeleft:
            stateMachine.TriggerEvent(Event::BadCardDetected);
          break;
          
          case UploadStatus::NoCard:
          case UploadStatus::StorageIsFull:
          // todo storage is full screen
          break;
        }
      break;

    case State::BatteryIndication:
      screenHandler.Handle(now);
      if (screenHandler.IsGroupFinished())
      {
        stateMachine.TriggerEvent(Event::IndicationFinished);
      }
      break;

    case State::GoodCardIndication:
      musicPlayer.Loop();
      backlight.TickDiscrete(now);
      if (backlight.IsDiscreteEnded() && musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::IndicationFinished);
      }
      break;

    case State::BadCardIndication:
      musicPlayer.Loop();
      if (musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::IndicationFinished);
      }
      break;

    case State::SlotMachine:
      break;

    case State::Ending:
      screenHandler.Handle(now);
      musicPlayer.Loop();
      if (screenHandler.IsGroupFinished() && musicPlayer.IsFinished())
      {
        stateMachine.TriggerEvent(Event::IndicationFinished);
      }
      break;
  }
}

void StartGoodCardIndication()
{
  musicPlayer.PlayBeep();
  backlight.ResetDiscrete();
  backlight.SetTotalDicsreteCycles(5U);
  rfid.end();
}

void StartBadCardIndication()
{
  rfid.end();
  musicPlayer.PlayOops();
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
  cardController.Reset();
  rfid.begin(9600);
  screenHandler.SetScreenset(&idleScreenset);
}

void StartBatteryIndication()
{
  screenHandler.SetScreenset(&batteryScreenset);
}
