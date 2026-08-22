#pragma once

#include <BuzzerMelody.h>

class MusicPlayer
{
  public:
  
    MusicPlayer(BuzzerMelody* melodies, uint8_t melodiesCount, uint8_t initialIndex) :
      mMelodies{ melodies},
      mMelodiesCount(melodiesCount),
      mCurrentMelodyIndex(initialIndex)
    {
    }

    void ChangeMelody()
    {
      mCurrentMelodyIndex = (mCurrentMelodyIndex < (mMelodiesCount - 1)) ? mCurrentMelodyIndex + 1 : 0;
    }

    void Play()
    {
      GetCurrentMelody()->play();
    }

    void Stop()
    {
      GetCurrentMelody()->stop();
    }

    void Loop()
    {
      GetCurrentMelody()->loop();
    }

    bool IsActive()
    {
      return GetCurrentMelody()->getState() != BuzzerMelody::IDLE;
    }

  private:
    BuzzerMelody* GetCurrentMelody()
    {
      return &mMelodies[mCurrentMelodyIndex];
    }
 
    BuzzerMelody* mMelodies;
    uint8_t mMelodiesCount;
    uint8_t mCurrentMelodyIndex;
};
