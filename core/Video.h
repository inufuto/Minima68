#pragma once

constexpr auto ColorCount = 16;

constexpr auto WindowWidth = 32;
constexpr auto VramWidth = WindowWidth * 2;
constexpr auto StatusAreaHeight = 2;
constexpr auto FiledAreaHeight = 24;
constexpr auto VramHeight = StatusAreaHeight + FiledAreaHeight;

constexpr auto DotsPerByte = 2;
constexpr auto TileWidth = 4;
constexpr auto TileHeight = 8;
constexpr auto TileWidthInBytes = TileWidth / DotsPerByte;
constexpr auto TilePatternSize = TileWidthInBytes * TileHeight;
constexpr auto SpriteWidth = TileWidth * 2;
constexpr auto SpriteHeight = TileHeight * 2;

constexpr auto XResolution = TileWidth * WindowWidth;
constexpr auto YResolution = TileHeight * VramHeight;
constexpr auto SpriteRangeX = TileWidth * WindowWidth;
constexpr auto SpriteRangeY = TileHeight * FiledAreaHeight;
constexpr auto SpriteCount = 64;
constexpr auto MaxHorizontalSpriteCount = 16;
