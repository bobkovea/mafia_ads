#pragma once

#include <BuzzerMelody.h>
#include "SuperMarioBrosTheme.h"
#include "HarryPotterTheme.h"

#define MELODIES_COUNT 2U

class Player
{
  public:
  
    Player(int buzzerPinNum) :
      harry(buzzerPinNum, Harry::melodyLength, Harry::melody),
      mario(buzzerPinNum, Mario::melodyLength, Mario::melody),
      melodies{ &harry, &mario },
      currentMelodyIndex(0U)
    {
    }

    void ChangeMelody()
    {
      currentMelodyIndex = (currentMelodyIndex < (MELODIES_COUNT - 1)) ? currentMelodyIndex + 1 : 0;
    }

    void Play()
    {
      melodies[currentMelodyIndex]->play();
    }

    void Stop()
    {
      melodies[currentMelodyIndex]->stop();
    }

    void Loop()
    {
      melodies[currentMelodyIndex]->loop();
    }

    bool IsActive()
    {
      return melodies[currentMelodyIndex]->getState() != BuzzerMelody::IDLE;
    }

  private:

    BuzzerMelody harry;
    BuzzerMelody mario;
    BuzzerMelody* melodies[MELODIES_COUNT];
    uint8_t currentMelodyIndex;
};
