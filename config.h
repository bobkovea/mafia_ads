#pragma once

#include "statemachine.h"

static constexpr uint8_t IsrPin = 2;
static constexpr uint8_t BuzzerPin = 3;
static constexpr uint8_t BacklightPin = 9;
static constexpr uint8_t LcdI2cAddress = 0x27;
static constexpr uint8_t LcdRowsCount = 2;
static constexpr uint8_t LcdColsCount = 16;

static const char* loadingMessages[] =
{
  "Поиск в базе... ",
  "Расчет модели...",
  "Подбор роли...  "
};

// Автоматически вычисляем размер массива
static constexpr uint8_t LoadingMessagesCount = sizeof(loadingMessages) / sizeof(loadingMessages[0]);
static_assert(LoadingMessagesCount > 0, "Operations array cannot be empty");

// ============ Прототипы действий ============
void StartLoading();
void StartEnding();
void StartCard();

// ============ Таблица переходов ============
const Transition transitions[] = {
  { State::Card, (uint8_t)Event::CardDetected, State::Loading, StartLoading },
  { State::Loading, (uint8_t)Event::LoadingFinished, State::Ending, StartEnding },
  { State::Ending, (uint8_t)Event::EndingFinished, State::Card, StartCard },
};

static constexpr uint8_t TransitionsCount = sizeof(transitions) / sizeof(transitions[0]);
static_assert(TransitionsCount > 0, "Transitions array cannot be empty");
