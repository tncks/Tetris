
#include "TetrisTitle.h"

BOOL Title::bButtonHover[2] = { FALSE, };
BOOL Title::bButtonClick[2] = { FALSE, };

VOID Title::Draw(HDC hDC) {
	RECT rcRect = { 0, };

	MyClear(FALSE);

	// Tetris
	MyRectangle(100, 100, 600, 100, 0x00FFFFFF);

	// 게임 시작
	MyRectangle(300, 250, 200, 100, bButtonHover[0] ? MyRGB(RGB(200, 200, 200)) : MyRGB(RGB(255, 255, 255)));
	// 게임 종료
	MyRectangle(300, 375, 200, 100, bButtonHover[1] ? MyRGB(RGB(200, 200, 200)) : MyRGB(RGB(255, 255, 255)));

	MyFinish();

	rcRect.left = 100;
	rcRect.top = 100;
	rcRect.right = 700;
	rcRect.bottom = 200;
	DrawText(hDC, TEXT("Tetris"), lstrlenW(TEXT("Tetris")), &rcRect, DT_CENTER | DT_VCENTER);
	
	rcRect.left = 300;
	rcRect.top = 250;
	rcRect.right = 500;
	rcRect.bottom = 350;
	DrawText(hDC, TEXT("게임 시작"), lstrlenW(TEXT("게임 시작")), &rcRect, DT_CENTER | DT_VCENTER);

	rcRect.top = 375;
	rcRect.bottom = 475;
	DrawText(hDC, TEXT("게임 종료"), lstrlenW(TEXT("게임 종료")), &rcRect, DT_CENTER | DT_VCENTER);

	return;
}

VOID Title::CheckButton(LPARAM lParam) {
	INT X = 0, Y = 0;

	X = LOWORD(lParam);
	Y = HIWORD(lParam);

	if (300 < X && X < 500) {
		if (Y > 250 && Y < 350) {
			bButtonHover[0] = TRUE;
			bButtonHover[1] = FALSE;
		} else if (Y > 375 && Y < 475) {
			bButtonHover[1] = TRUE;
			bButtonHover[0] = FALSE;
		} else {
			bButtonHover[0] = FALSE;
			bButtonHover[1] = FALSE;
		}
	} else {
		bButtonHover[0] = FALSE;
		bButtonHover[1] = FALSE;
	}

	return;
}

INT Title::CheckButtonClick(VOID) {
	if (bButtonHover[0]) {
		return 1;
	} else if (bButtonHover[1]) {
		return 2;
	} else {
		return 0;
	}

	return 0;
}