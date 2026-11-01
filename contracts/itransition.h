// contracts/itransition.h
#pragma once
#include <stdint.h>

enum class TransitionPhase : uint8_t
{
  FadeOut,   // уводим в «скрытое» состояние
  FadeIn     // возвращаем в «видимое»
};

class ITransition
{
  public:
    // Запустить анимацию (сбросить внутреннее состояние).
    virtual void Start(const uint32_t now) = 0;

    // Продвинуть анимацию. phase говорит, в какую сторону идём.
    virtual void Tick(const uint32_t now, const TransitionPhase phase) = 0;

    // Достигнуто «скрытое» состояние (конец FadeOut).
    virtual bool IsHidden() const = 0;

    // Достигнуто «видимое» состояние (конец FadeIn).
    virtual bool IsVisible() const = 0;
};