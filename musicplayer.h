#pragma once

#include <BuzzerMelody.h>
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"
#include "melodies/doorbeep.h"
#include "rolemanager.h"

template <uint8_t buzzerPin>
class MusicPlayer
{
  public:

    void PlayBeep()
    {
      _currentMelody = &_beep;
      _currentMelody->play();
    }

    void PlayRole(const MafiaRole role)
    {
      switch (role)
      {
        case MafiaRole::Citizen:
          _currentMelody = &_citizen;
          break;
        case MafiaRole::Sheriff:
          _currentMelody = &_sheriff;
          break;
        case MafiaRole::Mafia:
          _currentMelody = &_mafia;
          break;
        case MafiaRole::Don:
          _currentMelody = &_don;
          break;

        default:
          break;
      }

      _currentMelody->play();
    }

    void Loop()
    {
      _currentMelody->loop();
    }

    bool IsFinished()
    {
      return _currentMelody->getState() == BuzzerMelody::IDLE;
    }

  private:

    BuzzerMelody _beep = BuzzerMelody(buzzerPin, DoorBeep::melodyLength, DoorBeep::melody);
    BuzzerMelody _citizen = BuzzerMelody(buzzerPin, Aha::melodyLength, Aha::melody);
    BuzzerMelody _sheriff = BuzzerMelody(buzzerPin, Pirates::melodyLength, Pirates::melody);
    BuzzerMelody _mafia = BuzzerMelody(buzzerPin, PinkPanther::melodyLength, PinkPanther::melody);
    BuzzerMelody _don = BuzzerMelody(buzzerPin, Godfather::melodyLength, Godfather::melody);

    BuzzerMelody* _currentMelody = &_citizen;
};
