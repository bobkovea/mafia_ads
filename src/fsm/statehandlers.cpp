#include <Arduino.h>
#include "statehandlers.h"
#include "..\app\peripherals.h"
#include "..\app\displays.h"
#include "statemachine_global.h"

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

void StartGoodCardIndication()
{
    musicPlayer.SetTrack(Track::Beep);
    backlight.ResetDiscrete();
    backlight.SetTotalDicsreteCycles(5U);
    rfid.end();
}

void StartBadCardIndication()
{
    rfid.end();
    musicPlayer.SetTrack(Track::Oops);
    screenHandler.SetScreenset(&timeleftScreenset);
    timeleftScreen.SetCardTimestamp(cardController.GetLastCardTimestamp());
}

void StartSlotMachine()
{
}

void StartEnding()
{
    musicPlayer.SetTrack(Track::Godfather);
    screenHandler.SetScreenset(&endingScreenset);
}

void HandleState(const State state, const uint32_t now)
{
    switch (state)
    {
    case State::Idle:
        screenHandler.Handle(now);
        switch (cardController.Poll(now))
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
            stateMachine.TriggerEvent(Event::IndicationFinished);
        break;

    case State::GoodCardIndication:
        musicPlayer.Loop();
        backlight.TickDiscrete(now);
        if (backlight.IsDiscreteEnded() && musicPlayer.IsFinished())
            stateMachine.TriggerEvent(Event::IndicationFinished);
        break;

    case State::BadCardIndication:
        screenHandler.Handle(now);
        musicPlayer.Loop();
        if (screenHandler.IsGroupFinished() && musicPlayer.IsFinished())
            stateMachine.TriggerEvent(Event::IndicationFinished);
        break;

    case State::SlotMachine:
        break;

    case State::Ending:
        screenHandler.Handle(now);
        musicPlayer.Loop();
        if (screenHandler.IsGroupFinished() && musicPlayer.IsFinished())
            stateMachine.TriggerEvent(Event::IndicationFinished);
        break;
    }
}