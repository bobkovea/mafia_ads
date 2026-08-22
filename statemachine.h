#pragma once

#include <stdint.h>

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

// ============ Прототип действия ============
typedef void (*ActionCallback)();

// ============ Структура перехода ============
struct Transition
{
  State currentState;
  uint8_t event;
  State nextState;
  ActionCallback action;
};

// ============ Класс конечного автомата ============
class StateMachine
{
  public:
    StateMachine(const Transition* transitions, const uint8_t transitionsCount, const State initialState)
      : mTransitions(transitions),
        mTransitionsCount(transitionsCount),
        mCurrentState(initialState),
        mEvents((uint8_t)Event::None)
    {
    }

    // Генерация события
    void TriggerEvent(Event event)
    {
      mEvents |= (uint8_t)event;
    }

    // Обработка переходов
    void Process()
    {
      if (mEvents != (uint8_t)Event::None)
      {
        const uint8_t currentEvents = mEvents;
        mEvents = (uint8_t)Event::None;

        for (uint8_t i = 0; i < mTransitionsCount; i++)
        {
          const Transition& trans = mTransitions[i];

          const bool stateMatches = trans.currentState == mCurrentState;
          const bool eventMatches = (trans.event & currentEvents) != 0;

          if (stateMatches && eventMatches)
          {
            // Выполняем действие
            if (trans.action != nullptr)
            {
              trans.action();
            }

            // Переходим в новое состояние
            mCurrentState = trans.nextState;
            break;
          }
        }
      }
    }

    // Получение текущего состояния
    State GetCurrentState() const
    {
      return mCurrentState;
    }

  private:
    const Transition* mTransitions;
    uint8_t mTransitionsCount;
    volatile State mCurrentState;
    volatile uint8_t mEvents;

};
