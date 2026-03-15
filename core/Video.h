#pragma once

constexpr auto ColorCount = 16;

constexpr auto VramWidth = 32;
constexpr auto VramHeight = 26;

constexpr auto DotsPerByte = 2;
constexpr auto TileWidth = 4;
constexpr auto TileHeight = 8;
constexpr auto TileWidthInBytes = TileWidth / DotsPerByte;
constexpr auto TilePatternSize = TileWidthInBytes * TileHeight;
constexpr auto SpriteWidth = TileWidth * 2;
constexpr auto SpriteHeight = TileHeight * 2;
constexpr auto SpritePatternSize = SpriteWidth / DotsPerByte * SpriteHeight;

constexpr auto XResolution = TileWidth * VramWidth;
constexpr auto YResolution = TileHeight * VramHeight;
constexpr auto SpriteRangeX = TileWidth * VramWidth;
constexpr auto SpriteRangeY = TileHeight * VramHeight;
constexpr auto SpriteCount = 64;
constexpr auto MaxHorizontalSpriteCount = 16;

struct SpriteAttribute
{
	uint8_t y;
	uint8_t x;
	uint8_t pattern;
	uint8_t dummy;
};