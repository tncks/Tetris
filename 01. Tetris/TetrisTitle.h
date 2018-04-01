#pragma once

#include "Graphics.h"

class Title {
private:
	static BOOL bButtonHover[2];
	static BOOL bButtonClick[2];

public:
	static VOID Draw(HDC hDC);
	static VOID CheckButton(LPARAM lParam);
	static INT CheckButtonClick(VOID);
};