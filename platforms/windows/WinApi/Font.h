#pragma once
#include <windows.h>
#include "../../../core/Uncopyable.h"

class Font : public Uncopyable
{
private:
	HFONT hFont;
public:
	~Font() override { if (hFont) { DeleteObject(hFont); } }
	HFONT HFont() const { return hFont; }
	void Create(const LOGFONT& logFont);
	void CreateFixed(int size);
};
