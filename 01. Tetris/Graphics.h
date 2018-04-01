#pragma once

#include <Windows.h>

extern BYTE *pMem;
extern HBITMAP hBack;

void MyClear(BOOL bWhite = TRUE);
UINT MyRGB(COLORREF RGB);
void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, UINT cColor);
void MyFinish(void);