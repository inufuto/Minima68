#pragma once

constexpr auto ColorCount = 16;

constexpr auto FieldWindowWidth = 32;
constexpr auto VideoWindowHeight = 25;
constexpr auto FieldAreaWidth = FieldWindowWidth * 2;
constexpr auto StatusAreaWidth = 8;
constexpr auto VramWidth = FieldAreaWidth + StatusAreaWidth;

constexpr auto DotsPerByte = 2;
constexpr auto TileWidth = 4;
constexpr auto TileHeight = 8;
constexpr auto TileWidthInBytes = TileWidth / DotsPerByte;
constexpr auto TilePatternSize = TileWidthInBytes * TileHeight;

static constexpr auto XResolution = TileWidth * (FieldWindowWidth + StatusAreaWidth);
static constexpr auto YResolution = TileHeight * VideoWindowHeight;
static constexpr auto SpriteRangeX = TileWidth * FieldWindowWidth;
static constexpr auto SpriteRangeY = YResolution;

constexpr auto TilePatternAddress = 0xe000;
constexpr auto TileMapAddress = 0xf000;
constexpr auto PaletteAddress = 0xf800;
constexpr auto VideoModeAddress = 0xf820;
