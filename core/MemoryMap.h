#pragma once

constexpr auto TilePatternAddress = 0xd000;
constexpr auto SpritePatternAddress = 0xe000;
constexpr auto TileMapAddress = 0xf000;
constexpr auto Page0Address = TileMapAddress;
constexpr auto Page1Address = 0xf400;
constexpr auto SpriteAttributeAddress = 0xf800;
constexpr auto PaletteAddress = 0xff00;
constexpr auto PageAddress = 0xff30;

constexpr auto ToneSampleAddress = 0xff40;
constexpr auto FrequencyAddress = 0xff46;
constexpr auto ToneVolumeAddress = 0xff4c;
constexpr auto EffectSampleAddress = 0xff52;
constexpr auto EffectRateAddress = 0xff54;
constexpr auto EffectVolumeAddress = 0xff55;

constexpr auto JoystickAddress = 0xff80;
constexpr auto KeyCodeAddress = 0xff81;
