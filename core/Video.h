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

static constexpr auto XResolution = TileWidth * WindowWidth;
static constexpr auto YResolution = TileHeight * VramHeight;
static constexpr auto SpriteRangeX = TileWidth * WindowWidth;
static constexpr auto SpriteRangeY = TileHeight * FiledAreaHeight;
