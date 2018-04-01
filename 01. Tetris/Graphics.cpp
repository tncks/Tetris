
#include "Graphics.h"

void MyClear(BOOL bWhite) {
	if (bWhite) {
		memset(pMem, 255, 800 * 600 * 4);
	} else {
		memset(pMem, 0, 800 * 600 * 4);
	}

	return;
}

UINT MyRGB(COLORREF RGB) {
	union {
		UINT iResult;
		BYTE ARGB[4];
	};

#pragma omp parallel
	{
		ARGB[3] = 0;
		ARGB[2] = static_cast<BYTE>(RGB);
		ARGB[1] = static_cast<BYTE>(RGB >> 8);
		ARGB[0] = static_cast<BYTE>(RGB >> 16);
	}

	return iResult;
}

void MyRectangle(UINT X, UINT Y, UINT iWidth, UINT iHeight, UINT cColor) {
#pragma omp parallel for
	for (INT x = X; x < static_cast<INT>(X + iWidth); x++) {
#pragma omp parallel for
		for (INT y = Y; y < static_cast<INT>(Y + iHeight); y++) {
			memcpy(reinterpret_cast<UINT *>(pMem + y * 800 * 4 + x * 4), &cColor, 4);
		}
	}

	return;
}

void MyFinish(VOID) {
	SetBitmapBits(hBack, 800 * 600 * 4, pMem);

	return;
}