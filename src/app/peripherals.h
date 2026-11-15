#pragma once

#include <SoftwareSerial.h>
#include "../config/pins.h"
#include "../config/timings.h"
#include "../config/storage.h"
#include "../card/cardreader.h"
#include "../card/cardstorage.h"
#include "../card/cardcontroller.h"
#include "../music/musicplayer.h"
#include "../adc/batterymanager.h"
#include "../display/lcd1602rus_hw.h"
#include "../display/backlight.h"

extern SoftwareSerial rfid;
extern CardReader cardReader;
extern CardStorage<StorageSize, TimeBetweenMs> cardStorage;
extern CardController cardController;

extern MusicPlayer<BuzzerPin> musicPlayer;
extern BatteryManager<VbatPin, VccMv> batteryManager;
extern Lcd1602rus_hw lcd;
extern Backlight<BacklightPin> backlight;