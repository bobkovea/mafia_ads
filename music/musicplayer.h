// musicplayer.h
#pragma once

#include <BuzzerMelody.h>
#include "melodies/godfather.h"
#include "melodies/pinkpanther.h"
#include "melodies/pirates.h"
#include "melodies/aha.h"
#include "melodies/doorbeep.h"
#include "melodies/oopsbeep.h"

enum class Track : uint8_t
{
  Beep,
  Oops,
  Aha,
  Pirates,
  PinkPanther,
  Godfather
};

struct MelodyData
{
  const Note* melody;
  uint16_t length;
};

template <uint8_t buzzerPin>
class MusicPlayer
{
  public:

  void SetTrack(const Track track)
  {
    const auto& t = Melodies[(uint8_t)track];
    Play(t.melody, t.length);
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
  
  template<size_t N>
  static constexpr MelodyData makeTrack(const Note (&arr)[N])
  {
    return { arr, static_cast<uint16_t>(N) };
  }

  static constexpr MelodyData Melodies[] = {
    makeTrack(DoorBeep::melody),
    makeTrack(OopsBeep::melody),
    makeTrack(Aha::melody),
    makeTrack(Pirates::melody),
    makeTrack(PinkPanther::melody),
    makeTrack(Godfather::melody)
  };

    void Play(const Note* melody, const uint16_t length)
    {
      _player.stop();
      _player.loadMelody(length, melody);
      _player.play();
    }

    BuzzerMelody _player = BuzzerMelody(buzzerPin, 0, nullptr);
};
