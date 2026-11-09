// musicplayer.h
#pragma once

#include <BuzzerMelody.h>
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"
#include "melodies/doorbeep.h"
#include "melodies/oopsbeep.h"
#include "../role/rolemanager.h"

template <uint8_t buzzerPin>
class MusicPlayer
{
  public:

    void PlayBeep()
    {
      Play(DoorBeep::melody, DoorBeep::melodyLength);
    }

    void PlayOops()
    {
      Play(OopsBeep::melody, OopsBeep::melodyLength);
    }

    //TODO сделать свой енум для мелодий, не привязываясь к RoleManager
    void PlayRole(const MafiaRole role)
    {
      switch (role)
      {
        case MafiaRole::Citizen: Play(Aha::melody,         Aha::melodyLength);         break;
        case MafiaRole::Sheriff: Play(Pirates::melody,     Pirates::melodyLength);     break;
        case MafiaRole::Mafia:   Play(PinkPanther::melody, PinkPanther::melodyLength); break;
        case MafiaRole::Don:     Play(Godfather::melody,   Godfather::melodyLength);   break;
        default: break;
      }
    }

    void Loop()
    {
      _player.loop();
    }

    bool IsFinished()
    {
      return _player.getState() == BuzzerMelody::IDLE;
    }

  private:

    void Play(const Note* melody, uint16_t length)
    {
      _player.stop();
      _player.loadMelody(length, melody);
      _player.play();
    }

    BuzzerMelody _player = BuzzerMelody(buzzerPin, 0, nullptr);
};
