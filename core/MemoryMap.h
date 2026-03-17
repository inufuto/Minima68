#pragma once

constexpr auto TilePatternAddress = 0xd000;
constexpr auto SpritePatternAddress = 0xe000;
constexpr auto TileMapAddress = 0xf000;
constexpr auto Page0Address = TileMapAddress;
constexpr auto Page1Address = 0xf400;
constexpr auto SpriteAttributeAddress = 0xf800;
constexpr auto PaletteAddress = 0xff00;
constexpr auto PageAddress = 0xff30;

constexpr auto ToneFrequencyAddress = 0xff40;
constexpr auto ToneVolumeAddress = 0xff46;
constexpr auto EffectFrequencyAddress = 0xff4c;
constexpr auto EffectVolumeAddress = 0xff4e;

constexpr auto JoystickAddress = 0xff80;
constexpr auto KeyCodeAddress = 0xff81;
