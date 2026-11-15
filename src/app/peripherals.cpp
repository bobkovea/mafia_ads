#include "peripherals.h"

SoftwareSerial rfid(CardReaderDataPin, CardReaderDataPin);
CardReader cardReader(rfid);
CardStorage<StorageSize, TimeBetweenMs> cardStorage;
CardController cardController(cardReader, cardStorage);

MusicPlayer<BuzzerPin> musicPlayer;
BatteryManager<VbatPin, VccMv> batteryManager;
Lcd1602rus_hw lcd(LcdI2cAddress, LcdColsCount, LcdRowsCount);
Backlight<BacklightPin> backlight;