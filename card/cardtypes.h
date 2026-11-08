#pragma once

// Мой:   0x42 0x00 0x9E 0x97 0x44
// Катин: 0x4A 0x00 0x59 0x8E 0x8F

static constexpr uint8_t IdBytesCount = 5;
using CardId = uint8_t[IdBytesCount];

struct IdAndTimestamp
{
  CardId id;
  uint32_t timestamp;
};

enum class UploadStatus : uint8_t
{
	NoCard,
	NewCardAdded,
	StorageIsFull,
	ExistingCardUpdated,
	ExistingCardTimeleft
};

struct UploadStatusAndTimestamp
{
	UploadStatus status;
	uint32_t timestamp;
};